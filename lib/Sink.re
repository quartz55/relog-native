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

let make: (Record.t => unit) => t = h => h;