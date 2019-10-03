open Types;

let send_to_sink = Sink.log;

module type LoggerInfo = {
  let namespace: option(string);
  let fields: Fields.t;
};

module type S = {
  let namespace: option(string);
  let fields: Fields.t;
  let trace: log('a);
  let debug: log('a);
  let dbg: log('a);
  let info: log('a);
  let warning: log('a);
  let warn: log('a);
  let error: log('a);
  let err: log('a);
};

module Make = (M: LoggerInfo) : S => {
  let namespace = M.namespace;
  let fields = M.fields;

  let log = (msgf, level) => {
    let record = Record.make(~msgf, ~namespace?, ~fields, level);
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

module Clone = (L: S, M: LoggerInfo) : S =>
  Make({
    let namespace =
      switch (L.namespace, M.namespace) {
      | (Some(ns), Some(ns2)) => Some(String.concat(".", [ns, ns2]))
      | (Some(ns), _)
      | (_, Some(ns)) => Some(ns)
      | _ => None
      };
    let fields = Fields.merge(L.fields, M.fields);
  });

let logger = (~namespace=?, ~fields=[], ()): (module S) =>
  (module
   Make({
     let namespace = namespace;
     let fields = Fields.of_list(fields);
   }));

let clone = (~namespace=?, ~fields=[], l: (module S)): (module S) =>
  (module
   Clone(
     (val l),
     {
       let namespace = namespace;
       let fields = Fields.of_list(fields);
     },
   ));