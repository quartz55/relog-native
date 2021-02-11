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
  | Field(string, json)
  | LazyField(string, unit => json);

let key =
  fun
  | Field(k, _)
  | LazyField(k, _) => k;
let value =
  fun
  | Field(_, v) => v
  | LazyField(_, v_fn) => v_fn();
let compare = (l, r) => String.compare(key(l), key(r));
let equal = (l, r) => String.equal(key(l), key(r));

let make = (k, v) => Field(k, v);
let of_tuple = ((k, v)) => make(k, v);

module type JsonType = {
  type t;
  let to_json: t => json;
};

module type T = {type t;};

module type CONVERTER = {
  type src;
  module Types: T with type t = src;
  let f: (string, src) => t;
  let v: src => json;
  module Infix: {let (<=>): (string, src) => t;};
  let (<=>): (string, src) => t;
};

module type CONVERTER_IN = {
  module Types: T;
  let v: Types.t => json;
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
module Str = String;

module Bool =
  Make({
    type t = bool;
    let to_json = x => `Bool(x);
  });

module List =
       (M: CONVERTER_IN)
       : (CONVERTER with type src := list(M.Types.t)) => {
  include Make({
    type t = list(M.Types.t);
    let to_json = x => `List(CCList.map(M.v, x));
  });
};
module IntList = List(Int);
module FloatList = List(Float);
module StringList = List(String);
module StrList = StringList;
module BoolList = List(Bool);

module Assoc =
  Make({
    type t = list((string, json));
    let to_json = x => `Assoc(x);
  });

module Lazy =
       (M: CONVERTER_IN)
       : (CONVERTER with type src := unit => M.Types.t) => {
  type src = unit => M.Types.t;
  module Types = {
    type t = unit => M.Types.t;
  };
  let v = fn => M.v(fn());
  let f = (k, _v) => LazyField(k, () => v(_v));
  module Infix = {
    let (<=>) = f;
  };
  include Infix;
};

module LazyInt = Lazy(Int);
module LazyFloat = Lazy(Float);
module LazyString = Lazy(String);
module LazyStr = LazyString;
module LazyBool = Lazy(Bool);

module LazyList =
       (M: CONVERTER_IN)
       : (CONVERTER with type src := unit => list(M.Types.t)) => {
  include Lazy({
    module Types = {
      type t = list(M.Types.t);
    };
    let v = x => `List(CCList.map(M.v, x));
  });
};
module LazyIntList = LazyList(Int);
module LazyFloatList = LazyList(Float);
module LazyStringList = LazyList(String);
module LazyStrList = LazyStringList;
module LazyBoolList = LazyList(Bool);

let null = Null.f;
let int = Int.f;
let float = Float.f;
let string = String.f;
let str = string;
let bool = Bool.f;
let int_list = IntList.f;
let float_list = FloatList.f;
let str_list = StrList.f;
let bool_list = BoolList.f;
let assoc = Assoc.f;
let lazy_int = LazyInt.f;
let lazy_float = LazyFloat.f;
let lazy_str = LazyStr.f;
let lazy_bool = LazyBool.f;
let lazy_int_list = LazyIntList.f;
let lazy_float_list = LazyFloatList.f;
let lazy_str_list = LazyStrList.f;
let lazy_bool_list = LazyBoolList.f;
