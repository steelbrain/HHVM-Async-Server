<?hh
async function DoIt(int $I):Awaitable<void>{
  await SleepWaitHandle::create(rand(1,5) * 1000000);
  $Client = stream_socket_client('localhost:9098');
  fwrite($Client, "HELL YEAH");
  await SleepWaitHandle::create(rand(5, 10) * 1000000);
  echo "SENT: $I\n";
}
async function DoItAll():Awaitable<void>{
  $I = 0;
  await SleepWaitHandle::create(1000000);
  $Vector = Vector{};
  for($i = 0; $i <= 5; ++$i){
    $Vector->add(DoIt(++$I)->getWaitHandle());
  }
  await GenVectorWaitHandle::create($Vector);
}
DoItAll()->getWaitHandle()->join();
