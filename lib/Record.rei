type t;

let make:
  (~ts: Ptime.t=?, ~namespace: string=?, ~msgf: Types.msgf('a), Level.t) => t;
let ts: t => Ptime.t;
let level: t => Level.t;
let msg: (t, Format.formatter) => unit;
let namespace: t => option(string);