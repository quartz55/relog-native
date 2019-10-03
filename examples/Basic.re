module F = Relog.Field;

module L = (
  val Relog.logger(
        ~namespace=__MODULE__,
        ~fields=[F.String.("overwrite" <=> "me")],
        (),
      )
);
module L2 = (
  val Relog.clone(
        ~namespace="Child",
        ~fields=[F.Int.("pid" <=> 1342)],
        (module L),
      )
);

let setup_logging = () => {
  let json_fmter = Relog.Formatter.json();
  let cli_fmter = Relog.Formatter.default(~color=true, ());

  let file_oc = open_out("json-ex.log");
  let cli_fmt = Format.std_formatter;
  let json_fmt = Format.formatter_of_out_channel(file_oc);

  let sink =
    Relog.Sink.make(record => {
      if (Relog.(Level.Infix.(Record.level(record) < Level.Debug))) {
        Format.fprintf(cli_fmt, "%a", cli_fmter, record);
      };
      Format.fprintf(json_fmt, "%a", json_fmter, record);
    });
  Relog.Sink.set(sink);
};

setup_logging();

L.trace(m => m("trace", ~fields=[F.str("new", "key")]));
L.debug(m => m("debug", ~fields=[F.String.f("overwrite", "meeee")]));
L.info(m => m("info", ~fields=[F.Int.("pid" <=> 2)]));
L.warn(m => m("warn %d", 1));
L.error(m => m("error @[<v 2>let inc x =@ x + 1@]"));

L2.trace(m => m("trace"));
L2.debug(m => m("debug"));
L2.info(m => m("info"));
L2.warn(m => m("warn %d", 2));
L2.error(m => m("error"));