module F = Relog.Field;

module L = (
  val Relog.logger(
        ~namespace=__MODULE__,
        ~fields=[F.int("pid", Unix.getpid())],
        (),
      )
);

let setup_logging = () => {
  let cli_fmter = Relog.Formatter.default(~color=true, ());
  let cli_fmt = Format.formatter_of_out_channel(stderr);

  let sink =
    Relog.Sink.make_async(record => {
      Unix.sleep(1); // Long running operation (e.g. network request)
      Format.fprintf(cli_fmt, "%a", cli_fmter, record);
    });
  Relog.Sink.set(sink);
};

setup_logging();

L.trace(m => m("trace", ~fields=[F.str("new", "key")]));
L.debug(m => m("debug", ~fields=[F.StrList.f("d", ["e", "b", "u", "g"])]));
L.info(m => m("info", ~fields=[F.Int.("fff" <=> 2)]));
L.warn(m => m("warn %d", 1));
L.error(m =>
  m("error @[<v 2>let inc x =@ x + 1@]", ~fields=[F.int("x", 5)])
);

print_endline("you should see this before logging");
print_endline("you should see this before logging");
print_endline("you should see this before logging");
print_endline("you should see this before logging");
print_endline("you should see this before logging");
