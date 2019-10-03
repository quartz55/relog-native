include Set.Make(Field);

let of_list = x => x |> List.to_seq |> of_seq;
let of_tuples = x => x |> List.to_seq |> Seq.map(Field.of_tuple) |> of_seq;
let merge = (l, r) => {
  to_seq(r)
  |> Seq.fold_left(
       (out, rx) =>
         mem(rx, out) ? remove(rx, out) |> add(rx) : add(rx, out),
       l,
     );
};