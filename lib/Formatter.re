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

  let make = (~color=true, (), fmt, record) => {
    let msg = Record.msg(record);
    Format.fprintf(
      fmt,
      "@[%a@ %a@ %a@]@.@[%t@]@.",
      pp_ts,
      Record.ts(record),
      pp_level,
      Record.(color, level(record)),
      pp_namespace,
      Record.namespace(record),
      msg,
    );
  };
};

module Json = {
  let _opt_val_exn =
    fun
    | Some(v) => v
    | None => failwith("wooooo");
  let _opt_val_or_null =
    fun
    | Some(v) => "\"" ++ v ++ "\""
    | None => "null";

  let fmt = (fmt, record) => {
    let log = Record.msg(record);
    let ts = Record.ts(record);
    Format.fprintf(
      fmt,
      {|{"level": %d, "ts": %d, "namespace": %s, "message": "%t"}@.|},
      Level.verbosity(Record.level(record)),
      Ptime.to_span(ts) |> Ptime.Span.to_int_s |> _opt_val_exn,
      Record.namespace(record) |> _opt_val_or_null,
      log,
    );
  };
};

let default = Default.make;
let json = Json.fmt;