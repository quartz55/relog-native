type t = (Format.formatter, Record.t) => unit;

let default: (~color: bool=?, ~oneline: bool=?, unit) => t;
let json:
  (
    ~buf_size: int=?,
    ~fields_obj: string=?,
    ~ts_key: string=?,
    ~level_key: string=?,
    ~namespace_key: string=?,
    ~message_key: string=?,
    unit
  ) =>
  t;
