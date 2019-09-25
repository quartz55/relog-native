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

  let make = (~color=true, ~oneline=false, (), fmt, record) => {
    let msg = Record.msg(record);
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
    oneline ? Format.fprintf(fmt, " @[<h>") : Format.fprintf(fmt, "@.@[");
    Format.fprintf(fmt, "%t@]@.", msg);
  };
};

module Json = {
  let fmt = {
    let buf = Bi_outbuf.create(512);
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
      let json =
        Yojson.Basic.(
          `Assoc([
            ("level", `Int(level)),
            ("ts", `Int(ts)),
            ("namespace", ns),
            ("message", `String(msg)),
          ])
        );
      let out = json |> Yojson.Basic.to_string(~buf, ~std=true);
      Format.fprintf(fmt, "%s@.", out);
    };
  };
};

let default = Default.make;
let json = Json.fmt;