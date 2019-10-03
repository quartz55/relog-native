type t = (Format.formatter, Record.t) => unit;

module Default = {
  let pp_ts = {
    let tz_offset_s = Ptime_clock.current_tz_offset_s();
    Ptime.pp_rfc3339(~tz_offset_s?, ());
  };
  let pp_level = (fmt, (color, level)) => {
    let colorize = (level, str) => {
      let color =
        switch (level) {
        | Level.Error => Pastel.Red
        | Warn => Pastel.Yellow
        | Info => Pastel.Green
        | Debug => Pastel.Blue
        | Trace => Pastel.BlackBright
        };
      <Pastel color> str </Pastel>;
    };
    let level_fmt =
      level
      |> Level.to_string
      |> String.uppercase_ascii
      |> Printf.sprintf("%-5s")
      |> (color ? colorize(level) : (v => v));
    Format.fprintf(fmt, "[%s]", level_fmt);
  };
  let pp_namespace = (fmt, ns) => {
    switch (ns) {
    | Some(ns) => Format.fprintf(fmt, "[%s]", ns)
    | None => ()
    };
  };
  let pp_json = (fmt, json) =>
    Format.fprintf(fmt, "%s", Yojson.Basic.to_string(json));
  let pp_field = (fmt, (key, field)) => {
    Format.fprintf(fmt, "@[%s=%a@]", key, pp_json, field);
  };
  let pp_fields = (fmt, fields) => {
    Format.pp_open_hbox(fmt, ());
    fields
    |> Fields.iter(f => {
         pp_field(fmt, Field.(key(f), value(f)));
         Format.pp_print_space(fmt, ());
       });
    Format.pp_close_box(fmt, ());
  };

  let make = (~color=true, ~oneline=false, (), fmt, record) => {
    let msg = Record.msg(record);
    let fields = Record.fields(record);
    Format.fprintf(
      fmt,
      "@[%a@ %a@ %a@]",
      pp_ts,
      Record.ts(record),
      pp_level,
      Record.(color, level(record)),
      pp_namespace,
      Record.namespace(record),
    );
    oneline
      ? Format.fprintf(fmt, " @[<h>@[%t@]%a@]", msg, pp_fields, fields)
      : Format.fprintf(fmt, " @[<h>%a@]@.@[%t@]", pp_fields, fields, msg);
    Format.fprintf(fmt, "@.");
  };
};

module Json = {
  let fmt =
      (
        ~buf_size=512,
        ~fields_obj=?,
        ~ts_key="ts",
        ~level_key="level",
        ~namespace_key="namespace",
        ~message_key="msg",
        (),
      ) => {
    let buf = Bi_outbuf.create(buf_size);
    (fmt, r) => {
      let level = Record.level(r) |> Level.verbosity;
      let ts =
        Record.ts(r)
        |> Ptime.to_span
        |> Ptime.Span.to_int_s
        |> (
          fun
          | Some(ts) => ts
          | None => failwith("change me later")
        );
      let ns =
        Record.namespace(r)
        |> (
          fun
          | Some(ns) => `String(ns)
          | None => `Null
        );
      let msg = {
        Format.fprintf(Format.str_formatter, "%t", Record.msg(r));
        Format.flush_str_formatter();
      };
      let fields =
        Fields.to_seq(Record.fields(r))
        |> Seq.map(f => Field.(key(f), value(f)))
        |> List.of_seq;
      let fields =
        switch (fields_obj) {
        | Some(key) => [(key, `Assoc(fields))]
        | None => fields
        };
      let json =
        `Assoc([
          (level_key, `Int(level)),
          (ts_key, `Int(ts)),
          (namespace_key, ns),
          (message_key, `String(msg)),
          ...fields,
        ]);
      let out = json |> Yojson.Basic.to_string(~buf, ~std=true);
      Format.fprintf(fmt, "%s@.", out);
    };
  };
};

let default = Default.make;
let json = Json.fmt;