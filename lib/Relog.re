module Level = Level;
module Formatter = Formatter;
module Record = Record;
module Sink = Sink;
module Fields = Fields;
module Field = Field;

type msgf('a) = Types.msgf('a);
type log('a) = Types.log('a);
type record = Record.t;
type formatter = Formatter.t;
type level = Level.t;
type fields = Fields.t;
type field = Field.t;
type logger = (module Logger.S);

let logger = Logger.logger;
let clone = Logger.clone;
let child = clone;

module Logger = {
  module type S = Logger.S;
};
