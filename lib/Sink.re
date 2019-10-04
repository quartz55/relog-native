open Types;

type t = Record.t => unit;

let global_sink: ref(option(t)) = ref(None);

let is_set = () => global_sink^ != None;

let set = s => global_sink := Some(s);

let log = record =>
  switch (global_sink^) {
  | None => ()
  | Some(s) => s(record)
  };

let make = h => h;

let make_async = h => {
  let rec child_loop = rx => {
    switch (Marshal.from_channel(rx)) {
    | (record: Record.t) =>
      h(record);
      child_loop(rx);
    | exception End_of_file => () //print_endline("ASYNC_LOG rx closed")
    };
  };
  let (rx, wx) = Unix.pipe();
  switch (Unix.fork()) {
  | 0 =>
    Unix.close(wx);
    let rx = Unix.in_channel_of_descr(rx);
    child_loop(rx);
    close_in(rx);
    // print_endline("ASYNC_LOG child exiting...");
    exit(0);
  | pid =>
    Unix.close(rx);
    let wx = Unix.out_channel_of_descr(wx);
    at_exit(() => {
      close_out(wx);
      Unix.waitpid([], pid) |> ignore;
    });
    make((record: Record.t) => {
      Marshal.to_channel(wx, record, Marshal.[Closures])
    });
  };
};