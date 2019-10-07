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

module type CONVERTER = {
  type src;
  let f: (string, src) => t;
  module Infix: {let (<=>): (string, src) => t;};
  let (<=>): (string, src) => t;
};

module Make = (M: JsonType) : (CONVERTER with type src := M.t) => {
  type src = M.t;
  let f = (k, v) => Field(k, M.to_json(v));
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
module IntList =
  Make({
    type t = list(int);
    let to_json = x => `List(x |> CCList.map(v => `Int(v)));
  });
module FloatList =
  Make({
    type t = list(float);
    let to_json = x => `List(x |> CCList.map(v => `Float(v)));
  });
module StrList =
  Make({
    type t = list(string);
    let to_json = x => `List(x |> CCList.map(v => `String(v)));
  });
module BoolList =
  Make({
    type t = list(bool);
    let to_json = x => `List(x |> CCList.map(v => `Bool(v)));
  });

module Assoc =
  Make({
    type t = list((string, json));
    let to_json = x => `Assoc(x);
  });

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