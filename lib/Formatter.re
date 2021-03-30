type t = (Format.formatter, Record.t) => unit;

module Default = {
  let pp_ts = {
    let tz_offset_s = Ptime_clock.current_tz_offset_s();
    Ptime.pp_rfc3339(~tz_offset_s?, ());
  };
  let color_of_level =
    fun
    | Level.Error => "red"
    | Warn => "yellow"
    | Info => "green"
    | Debug => "blue"
    | Trace => "Black";
  let pp_level = (fmt, level) => {
    let pp = (fmt, level) => CCFormat.with_colorf(color_of_level(level), fmt, "%-5s", level |> Level.to_string |> String.uppercase_ascii);
    Format.fprintf(fmt, "[%a]", pp, level);
  };
  let pp_namespace = (fmt, ns) => {
    switch (ns) {
    | Some(ns) => Format.fprintf(fmt, "[%s]", ns)
    | None => ()
    };
  };
  let pp_json = (fmt, json) =>
    Format.fprintf(fmt, "%s", Yojson.Basic.to_string(json));
  let pp_fields = (fmt, fields) => {
    let rec pp' = curr =>
      switch (curr) {
      | Seq.Nil => ()
      | Seq.Cons(f, next) =>
        let (key, value) = Field.(key(f), value(f));
        Format.fprintf(fmt, "@[\"%s\"=%a@]", key, pp_json, value);
        let next = next();
        next != Seq.Nil ? Format.pp_print_space(fmt, ()) : ();
        pp'(next);
      };
    Format.pp_open_hbox(fmt, ());
    pp'(Fields.to_seq(fields, ()));
    Format.pp_close_box(fmt, ());
  };

  let make = (~color=true, ~oneline=false, (), fmt, record) => {
    if (color) {
      CCFormat.set_color_default(true);
      CCFormat.set_color_tag_handling(fmt);
    };
    let msg = Record.msg(record);
    let fields = Record.fields(record);
    Format.fprintf(
      fmt,
      "@[%a@ %a@ %a@]",
      pp_ts,
      Record.ts(record),
      pp_level,
      Record.level(record),
      pp_namespace,
      Record.namespace(record),
    );
    oneline
      ? Format.fprintf(fmt, " @[<h>(%a) @[%t@]@]", pp_fields, fields, msg)
      : Format.fprintf(fmt, " @[<h>(%a)@]@.@[%t@]", pp_fields, fields, msg);
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
