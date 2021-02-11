type t;

let is_set: unit => bool;
let set: t => unit;
let log: Record.t => unit;
let make: (Record.t => unit) => t;
let make_async: (Record.t => unit) => t;
