<?hh
require(__DIR__.'/Server.hh');
class MyServerSidedApp{
  public Server $Server;
  public function __construct(){
    $this->Server = $Server = new Server('localhost', 9001);
  }
  public async function OnConnect(resource $Client):Awaitable<void>{
    await Server::OnResponse($Client, async function(string $Data, resource $Client){
      echo "Client Sent Data: $Data\n";
    });
  }
  public async function Init():Awaitable<void>{
    await $this->Server->ListenInCoop(inst_meth($this, 'OnConnect'), async function(Server $Instance):Awaitable<void>{
      $ShouldClose = rand(0, 1);
      if($ShouldClose === 1){
        $Instance->Close();
      }
    });
  }
}
$App = new MyServerSidedApp();
$App->Init()->getWaitHandle()->join();
