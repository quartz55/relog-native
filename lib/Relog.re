module Level = Level;
module Formatter = Formatter;
module Record = Record;

open Types;

type record = Record.t;

let send_to_sink = Sink.log;

module type SINK = {
  type t;
  let set: t => unit;
  let make: (Record.t => unit) => t;
};
module Sink: SINK = {
  include Sink;
};

module type LoggerS = {let namespace: option(string);};

module type Logger = {
  let namespace: option(string);
  let trace: log('a);
  let debug: log('a);
  let dbg: log('a);
  let info: log('a);
  let warning: log('a);
  let warn: log('a);
  let error: log('a);
  let err: log('a);
};

module Logger = (M: LoggerS) : Logger => {
  let namespace = M.namespace;

  let log = (msgf, level) => {
    let record = Record.make(~msgf, ~namespace?, level);
    send_to_sink(record);
  };

  let trace = msgf => log(msgf, Level.Trace);
  let debug = msgf => log(msgf, Level.Debug);
  let dbg = debug;
  let info = msgf => log(msgf, Level.Info);
  let warning = msgf => log(msgf, Level.Warn);
  let warn = warning;
  let error = msgf => log(msgf, Level.Error);
  let err = error;
};

module Clone = (L: Logger, M: LoggerS) : Logger =>
  Logger({
    let namespace =
      switch (L.namespace, M.namespace) {
      | (Some(ns), Some(ns2)) => Some(String.concat(".", [ns, ns2]))
      | (Some(ns), _)
      | (_, Some(ns)) => Some(ns)
      | _ => None
      };
  });

let logger = (~namespace=?, ()): (module Logger) =>
  (module
   Logger({
     let namespace = namespace;
   }));

let clone = (~namespace=?, l: (module Logger)): (module Logger) =>
  (module
   Clone(
     (val l),
     {
       let namespace = namespace;
     },
   ));