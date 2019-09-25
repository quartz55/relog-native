type t = (Format.formatter, Record.t) => unit;

module Default: {let make: (~color: bool=?, unit) => t;};
module Json: {let fmt: t;};

let default: (~color: bool=?, unit) => t;
let json: t;