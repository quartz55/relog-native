open Types;

let now = () => Ptime_clock.now();

type t = {
  ts: Ptime.t,
  level: Level.t,
  msg: Format.formatter => unit,
  namespace: option(string),
};

let make:
  type a. (~ts: Ptime.t=?, ~namespace: string=?, ~msgf: msgf(a), Level.t) => t =
  (~ts=now(), ~namespace=?, ~msgf, level) => {
    ts,
    level,
    msg: fmt => msgf(msg => Format.fprintf(fmt, msg)),
    namespace,
  };

let ts = t => t.ts;
let level = t => t.level;
let msg = t => t.msg;
let namespace = t => t.namespace;