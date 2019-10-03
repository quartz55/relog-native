type t;

let make:
  (
    ~ts: Ptime.t=?,
    ~namespace: string=?,
    ~fields: Fields.t=?,
    ~msgf: Types.msgf('a),
    Level.t
  ) =>
  t;
let ts: t => Ptime.t;
let level: t => Level.t;
let msg: (t, Format.formatter) => unit;
let namespace: t => option(string);
let fields: t => Fields.t;