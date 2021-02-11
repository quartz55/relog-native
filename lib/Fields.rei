include Set.S with type elt := Field.t;
let of_list: list(Field.t) => t;
let of_tuples: list((string, Field.json)) => t;
let merge: (t, t) => t;
