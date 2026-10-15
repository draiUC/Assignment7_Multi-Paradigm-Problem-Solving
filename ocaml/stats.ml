(* stats.ml - Mean, median, mode (plus extras) on a list of integers.
   Paradigm: FUNCTIONAL. Immutable lists, no refs or mutable arrays, no loops;
   everything is built from map / fold / filter.

   Build: ocamlopt stats.ml -o stats
   Usage: ./stats 4 8 15 16 23 42      (or no args for a sample list) *)

(* Mean: a single fold computes the sum. *)
let mean xs =
  let sum = List.fold_left ( + ) 0 xs in
  float_of_int sum /. float_of_int (List.length xs)

(* Median: sort (returns a new list), then pick the middle element(s). *)
let median xs =
  let sorted = List.sort compare xs in
  let n = List.length sorted in
  let nth = List.nth sorted in
  if n mod 2 = 1 then float_of_int (nth (n / 2))
  else (float_of_int (nth (n / 2 - 1)) +. float_of_int (nth (n / 2))) /. 2.0

(* Frequency table as an association list, built with fold_left.
   Each step returns a NEW list; nothing is modified in place. *)
let frequencies xs =
  let bump table x =
    match List.assoc_opt x table with
    | None -> (x, 1) :: table
    | Some c -> (x, c + 1) :: List.remove_assoc x table
  in
  List.fold_left bump [] xs

(* Mode: find the max frequency with fold, then filter + map the winners.
   Returns (modes, frequency). *)
let modes xs =
  let table = frequencies xs in
  let best = List.fold_left (fun acc (_, c) -> max acc c) 0 table in
  let winners =
    table
    |> List.filter (fun (_, c) -> c = best)
    |> List.map fst
    |> List.sort compare
  in
  (winners, best)

(* Population standard deviation via map + fold. *)
let std_dev xs =
  let mu = mean xs in
  let squares = List.map (fun x -> (float_of_int x -. mu) ** 2.0) xs in
  sqrt (List.fold_left ( +. ) 0.0 squares /. float_of_int (List.length xs))

let show_list xs = "[" ^ String.concat ", " (List.map string_of_int xs) ^ "]"

(* Parse arguments without mutation: map over the tail of Sys.argv. *)
let parse_args () =
  Array.to_list Sys.argv |> List.tl
  |> List.map (fun s ->
         match int_of_string_opt s with
         | Some v -> v
         | None -> Printf.eprintf "Invalid integer: '%s'\n" s; exit 1)

let () =
  let xs =
    match parse_args () with
    | [] ->
        print_endline "(no arguments given; using sample data)";
        [ 4; 8; 15; 16; 23; 42; 8; 15; 8; 23; 15 ]
    | given -> given
  in
  let ms, freq = modes xs in
  let lo = List.fold_left min max_int xs and hi = List.fold_left max min_int xs in
  Printf.printf "Input:    %s\n" (show_list xs);
  Printf.printf "Sorted:   %s\n" (show_list (List.sort compare xs));
  Printf.printf "Count:    %d\n" (List.length xs);
  Printf.printf "Mean:     %.2f\n" (mean xs);
  Printf.printf "Median:   %.2f\n" (median xs);
  if freq = 1 then print_endline "Mode:     none (all values occur once)"
  else (
    Printf.printf "Mode:     %s\n" (show_list ms);
    Printf.printf "          (each appears %d times)\n" freq);
  Printf.printf "Min/Max:  %d / %d\n" lo hi;
  Printf.printf "Std dev:  %.2f\n" (std_dev xs)
