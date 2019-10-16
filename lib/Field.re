type json = [
  | `Null
  | `Bool(bool)
  | `Int(int)
  | `Float(float)
  | `String(string)
  | `Assoc(list((string, json)))
  | `List(list(json))
];

type t =
  | Field(string, json);

let key = (Field(k, _)) => k;
let value = (Field(_, v)) => v;
let compare = (l, r) => String.compare(key(l), key(r));
let equal = (l, r) => String.equal(key(l), key(r));

let make = (k, v) => Field(k, v);
let of_tuple = ((k, v)) => make(k, v);

module type JsonType = {
  type t;
  let to_json: t => json;
};

module type T = { type t; };

module type CONVERTER = {
  type src;
  module Types: T with type t = src;
  let f: (string, src) => t;
  let v: src => json;
  module Infix: {let (<=>): (string, src) => t;};
  let (<=>): (string, src) => t;
};

module Make = (M: JsonType) : (CONVERTER with type src := M.t) => {
  type src = M.t;
  module Types = {
    type t = M.t;
  };
  let v = M.to_json;
  let f = (k, v_) => Field(k, v(v_));
  module Infix = {
    let (<=>) = f;
  };
  include Infix;
};

module Null =
  Make({
    type t = unit;
    let to_json = () => `Null;
  });

module Int =
  Make({
    type t = int;
    let to_json = x => `Int(x);
  });

module Float =
  Make({
    type t = float;
    let to_json = x => `Float(x);
  });

module String =
  Make({
    type t = string;
    let to_json = x => `String(x);
  });

module Bool =
  Make({
    type t = bool;
    let to_json = x => `Bool(x);
  });

module List =
  Make({
    type t = list(json);
    let to_json = x => `List(x);
  });
module MakeList = (T: T, M: (CONVERTER with type src := T.t)): (CONVERTER with type src := list(M.Types.t)) => {
  include Make({
    type t = list(M.Types.t);
    let to_json = x => List.v(CCList.map(M.v, x));
  })
};
module IntList = MakeList({type t = int}, Int);
module FloatList = MakeList({type t = float}, Float);
module StrList = MakeList({type t = string}, String);
module BoolList = MakeList({type t = bool}, Bool);

module Assoc =
  Make({
    type t = list((string, json));
    let to_json = x => `Assoc(x);
  });

module Lazy =
  Make({
    type t = unit => json;
    let to_json = f => f();
  });
module MakeLazy = (T: T, M: (CONVERTER with type src := T.t)): (CONVERTER with type src := unit => M.Types.t) => {
  include Make({
    type t = unit => M.Types.t;
    let to_json = f => M.v(f());
  })
};
module LazyInt = MakeLazy({type t = int}, Int);
module LazyFloat = MakeLazy({type t = float}, Float);
module LazyStr = MakeLazy({type t = string}, String);
module LazyBool = MakeLazy({type t = bool}, Bool);

let null = Null.f;
let int = Int.f;
let float = Float.f;
let string = String.f;
let str = string;
let bool = Bool.f;
let list = List.f;
let int_list = IntList.f;
let float_list = FloatList.f;
let str_list = StrList.f;
let bool_list = BoolList.f;
let assoc = Assoc.f;
let lazy_ = Lazy.f;
let lazy_int = LazyInt.f;
let lazy_float = LazyFloat.f;
let lazy_str = LazyStr.f;
let lazy_bool = LazyBool.f;