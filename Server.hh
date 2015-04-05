<?hh //strict
class Server{
  public function __construct(public string $Address, public int $Port){}
  public async function Listen((function(resource):Awaitable<void>) $Callback):Awaitable<void>{
    $Socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
    socket_bind($Socket, $this->Address, $this->Port);
    socket_listen($Socket, 0);
    register_shutdown_function(function() use($Socket){
      socket_close($Socket);
    });
    while(is_resource($Socket)){
      $Callback(socket_accept($Socket))->getWaitHandle();
    }
  }
  public static async function OnResponse(resource $Client, (function(string, resource):Awaitable<void>) $Callback):Awaitable<void>{
    while(is_resource($Client) && !feof($Client)){
      $Content = fgets($Client);
      if(is_string($Content)){
        await $Callback($Content, $Client);
      } else break;
    }
    socket_close($Client);
  }
}
