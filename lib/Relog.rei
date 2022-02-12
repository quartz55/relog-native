module Level = Level;
module Formatter = Formatter;
module Record = Record;
module Sink: {
  type t;
  let set: t => unit;
  let make: (Record.t => unit) => t;
  let make_mutex: (Record.t => unit) => t;
};
module Fields = Fields;
module Field = Field;

type logger = (module Logger.S);
type msgf('a) = Types.msgf('a);
type log('a) = Types.log('a);
type record = Record.t;
type formatter = Formatter.t;
type level = Level.t;
type fields = Fields.t;
type field = Field.t;

let logger: (~namespace: string=?, ~fields: list(Field.t)=?, unit) => logger;
let clone: (~namespace: string=?, ~fields: list(Field.t)=?, logger) => logger;
let child: (~namespace: string=?, ~fields: list(Field.t)=?, logger) => logger;

module Logger: {module type S = Logger.S;};
