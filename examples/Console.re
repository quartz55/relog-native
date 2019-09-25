module L = (val Relog.logger(~namespace=__MODULE__, ()));
module L2 = (val Relog.clone(~namespace="child", (module L)));

let setup_logging = () => {
  let json_fmter = Relog.Formatter.json;
  let cli_fmter = Relog.Formatter.default(~color=true, ());

  let file_oc = open_out("lmao.log");
  let cli_fmt = Format.std_formatter;
  let json_fmt = Format.formatter_of_out_channel(file_oc);

  let sink =
    Relog.Sink.make(record => {
      Format.fprintf(cli_fmt, "%a", cli_fmter, record);
      Format.fprintf(json_fmt, "%a", json_fmter, record);
    });
  Relog.Sink.set(sink);
};

setup_logging();

L.trace(m => m("trace"));
L.debug(m => m("debug"));
L.info(m => m("info"));
L.warn(m => m("warn"));
L.error(m => m("error"));

L2.trace(m => m("trace"));
L2.debug(m => m("debug"));
L2.info(m => m("info"));
L2.warn(m => m("warn"));
L2.error(m => m("error"));