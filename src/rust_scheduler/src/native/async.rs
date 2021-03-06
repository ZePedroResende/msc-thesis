use rayon::prelude::*;
use rust_scheduler::native::data::Data;
use rust_scheduler::native::scheduler::scheduler;
use std::sync::mpsc::channel;
//use tokio::sync::oneshot;

fn main() {
    let d: Vec<Data> = (0..200)
        .into_par_iter()
        .map(|_| Data::new(1000, 2000, 1000))
        .collect::<Vec<Data>>();
    //let (sender, receiver) = oneshot();
    let (sender, receiver) = channel();
    //scheduler(d.first().unwrap().clone());

    let _a: Vec<_> = d
        .into_par_iter()
        .map_with(sender, |s, mut d| s.send(scheduler(&mut d)))
        .collect();

    let b: Vec<_> = receiver.iter().collect();

    println!("{:?}", b);
}
