type t =
  | Trace
  | Debug
  | Info
  | Warn
  | Error;

let verbosity: t => int;

let compare: (t, t) => int;
let equal: (t, t) => bool;

module Infix: {
  let (==): (t, t) => bool;
  let (>=): (t, t) => bool;
  let (<=): (t, t) => bool;
  let (>): (t, t) => bool;
  let (<): (t, t) => bool;
};

let of_verbosity: int => option(t);
let of_string: string => option(t);

let to_string: t => string;