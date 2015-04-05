<?hh //strict
class Server{
  public resource $Socket;
  public function __construct(public string $Address, public int $Port){
    $Socket = $this->Socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
    register_shutdown_function(function() use ($Socket){
      if(is_resource($Socket)) fclose($Socket);
    });
  }
  public function Close():void{
    fclose($this->Socket);
  }
  public async function Listen((function(resource):Awaitable<void>) $Callback):Awaitable<void>{
    if(!is_resource($this->Socket)){
      // It's closed...
      return ;
    }
    socket_bind($this->Socket, $this->Address, $this->Port);
    socket_listen($this->Socket, 100);
    $Handles = Vector{};
    while(is_resource($this->Socket)){
      try {
        $Handles->add($Callback(socket_accept($this->Socket))->getWaitHandle());
        if($Handles->count() === 2){
          await GenVectorWaitHandle::create($Handles);
          $Handles->clear();
        }
      } catch(Exception $e){} // Let the server run!
    }
  }
  public static async function OnResponse(resource $Socket, (function(string, resource):Awaitable<void>) $Callback):Awaitable<void>{
    while (is_resource($Socket) && !feof($Socket)){
      $select = await stream_await($Socket, STREAM_AWAIT_READ, 5);
      switch ($select) {
        case STREAM_AWAIT_READY:
          $Data = stream_get_contents($Socket);
          if(!is_bool($Data)){
            await $Callback($Data, $Socket);
          }
          break;
        case STREAM_AWAIT_CLOSED:
        return ;
        // Do nothing for timeouts
        default:
      }
    }
  }
  public async function ListenInCoop((function(resource):Awaitable<void>) $First, (function(Server):Awaitable<void>) $Second):Awaitable<void>{
    await GenArrayWaitHandle::create([$Second($this), $this->Listen($First)]);
  }
}
