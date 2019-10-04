type json = [
  | `Assoc(list((string, json)))
  | `Bool(bool)
  | `Float(float)
  | `Int(int)
  | `List(list(json))
  | `Null
  | `String(string)
];

type t;
let key: t => string;
let value: t => json;

let compare: (t, t) => int;
let equal: (t, t) => bool;

let make: (string, json) => t;
let of_tuple: ((string, json)) => t;

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

module Make: (M: JsonType) => CONVERTER with type src := M.t;
module Null: CONVERTER with type src := unit;
module Int: CONVERTER with type src := int;
module Float: CONVERTER with type src := float;
module String: CONVERTER with type src := string;
module Bool: CONVERTER with type src := bool;
module List: CONVERTER with type src := list(json);
module IntList: CONVERTER with type src := list(int);
module FloatList: CONVERTER with type src := list(float);
module StrList: CONVERTER with type src := list(string);
module BoolList: CONVERTER with type src := list(bool);
module Assoc: CONVERTER with type src := list((string, json));

let null: (string, unit) => t;
let int: (string, int) => t;
let float: (string, float) => t;
let string: (string, string) => t;
let str: (string, string) => t;
let bool: (string, bool) => t;
let list: (string, list(json)) => t;
let assoc: (string, list((string, json))) => t;