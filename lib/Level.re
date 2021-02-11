type t =
  | Trace
  | Debug
  | Info
  | Warn
  | Error;

let verbosity =
  fun
  | Trace => 5
  | Debug => 4
  | Info => 3
  | Warn => 2
  | Error => 1;

let compare = (lhs, rhs) => Stdlib.compare(verbosity(lhs), verbosity(rhs));
let equal = (l, r) => compare(l, r) == 0;

module Infix = {
  let (==) = equal;
  let (>=) = (l, r) => compare(l, r) >= 0;
  let (<=) = (l, r) => compare(l, r) <= 0;
  let (>) = (l, r) => compare(l, r) > 0;
  let (<) = (l, r) => compare(l, r) < 0;
};

let of_verbosity = v =>
  switch (v) {
  | _ when v > 4 => Some(Trace)
  | 4 => Some(Debug)
  | 3 => Some(Info)
  | 2 => Some(Warn)
  | 1 => Some(Error)
  | _ => None
  };

let of_string =
  fun
  | "trace" => Some(Trace)
  | "debug" => Some(Debug)
  | "info" => Some(Info)
  | "warn" => Some(Warn)
  | "error" => Some(Error)
  | _ => None;

let to_string =
  fun
  | Trace => "trace"
  | Debug => "debug"
  | Info => "info"
  | Warn => "warn"
  | Error => "error";
