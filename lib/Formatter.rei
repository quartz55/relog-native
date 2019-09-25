type t = (Format.formatter, Record.t) => unit;

let default: (~color: bool=?, ~oneline: bool=?, unit) => t;
let json: t;