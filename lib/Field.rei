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

module Make: (M: JsonType) => CONVERTER with type src := M.t;
module Null: CONVERTER with type src := unit;
module Int: CONVERTER with type src := int;
module Float: CONVERTER with type src := float;
module String: CONVERTER with type src := string;
module Str: CONVERTER with type src := string;
module Bool: CONVERTER with type src := bool;
module List: (M: CONVERTER_IN) => CONVERTER with type src := list(M.Types.t);
module IntList: CONVERTER with type src := list(int);
module FloatList: CONVERTER with type src := list(float);
module StringList: CONVERTER with type src := list(string);
module StrList: CONVERTER with type src := list(string);
module BoolList: CONVERTER with type src := list(bool);
module Assoc: CONVERTER with type src := list((string, json));
module Lazy:
  (M: CONVERTER_IN) => CONVERTER with type src := unit => M.Types.t;
module LazyInt: CONVERTER with type src := unit => int;
module LazyFloat: CONVERTER with type src := unit => float;
module LazyString: CONVERTER with type src := unit => string;
module LazyStr: CONVERTER with type src := unit => string;
module LazyBool: CONVERTER with type src := unit => bool;
module LazyList:
  (M: CONVERTER_IN) => CONVERTER with type src := unit => list(M.Types.t);
module LazyIntList: CONVERTER with type src := unit => list(int);
module LazyFloatList: CONVERTER with type src := unit => list(float);
module LazyStringList: CONVERTER with type src := unit => list(string);
module LazyStrList: CONVERTER with type src := unit => list(string);
module LazyBoolList: CONVERTER with type src := unit => list(bool);

let null: (string, unit) => t;
let int: (string, int) => t;
let float: (string, float) => t;
let string: (string, string) => t;
let str: (string, string) => t;
let bool: (string, bool) => t;
let int_list: (string, list(int)) => t;
let float_list: (string, list(float)) => t;
let str_list: (string, list(string)) => t;
let bool_list: (string, list(bool)) => t;
let assoc: (string, list((string, json))) => t;
let lazy_int: (string, unit => int) => t;
let lazy_float: (string, unit => float) => t;
let lazy_str: (string, unit => string) => t;
let lazy_bool: (string, unit => bool) => t;
let lazy_int_list: (string, unit => list(int)) => t;
let lazy_float_list: (string, unit => list(float)) => t;
let lazy_str_list: (string, unit => list(string)) => t;
let lazy_bool_list: (string, unit => list(bool)) => t;
