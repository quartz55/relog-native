module Level = Level;
module Formatter = Formatter;
module Record = Record;
module Sink: {
  type t;
  let set: t => unit;
  let make: (Record.t => unit) => t;
};

type record = Record.t;

module type LoggerS = {let namespace: option(string);};

module type Logger = {
  let namespace: option(string);
  let trace: Types.log('a);
  let debug: Types.log('a);
  let dbg: Types.log('a);
  let info: Types.log('a);
  let warning: Types.log('a);
  let warn: Types.log('a);
  let error: Types.log('a);
  let err: Types.log('a);
};

module Logger: (M: LoggerS) => Logger;
module Clone: (L: Logger, M: LoggerS) => Logger;

let logger: (~namespace: string=?, unit) => (module Logger);
let clone: (~namespace: string=?, (module Logger)) => (module Logger);