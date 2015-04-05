<?hh
require(__DIR__.'/Server.hh');
$Server = new Server('localhost', 9098);
$Server->Listen(async function(resource $Client):Awaitable<void>{
  echo "Client Connected\n";
  await Server::OnResponse($Client, async function(string $Data, resource $Client):Awaitable<void>{
    echo "Client Sent Data: $Data\n";
  });
  echo "Client Disconnected\n";
})->getWaitHandle()->join();
