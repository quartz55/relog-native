open Types;

let now = () => Ptime_clock.now();

let nop_fmt = Format.make_formatter((_, _, _) => (), () => ());
let ignore_msgf: format4('a, Format.formatter, unit, unit) => 'a =
  msgf => Format.ifprintf(nop_fmt, msgf);

type t = {
  ts: Ptime.t,
  level: Level.t,
  msgf: Format.formatter => unit,
  namespace: option(string),
  m_fields: Fields.t,
  get_msg_fields: unit => Fields.t,
  mutable fields: option(Fields.t),
};

let make:
  type a.
    (
      ~ts: Ptime.t=?,
      ~namespace: string=?,
      ~fields: Fields.t=?,
      ~msgf: msgf(a),
      Level.t
    ) =>
    t =
  (~ts=now(), ~namespace=?, ~fields=Fields.empty, ~msgf, level) => {
    {
      ts,
      level,
      msgf: fmt => msgf((~fields=?, msg) => {Format.fprintf(fmt, msg)}),
      get_msg_fields: () => {
        let t = ref(Fields.empty);
        msgf((~fields=?, msgf) => {
          switch (fields) {
          | None => ()
          | Some(f) => t := Fields.of_list(f)
          };
          ignore_msgf(msgf);
        });
        t^;
      },
      m_fields: fields,
      fields: None,
      namespace,
    };
  };

let ts = t => t.ts;
let level = t => t.level;
let msg = t => t.msgf;
let namespace = t => t.namespace;
let fields = t =>
  switch (t.fields) {
  | None =>
    let f = Fields.merge(t.m_fields, t.get_msg_fields());
    t.fields = Some(f);
    f;
  | Some(f) => f
  };
