HHVM-Async-Server
=================
Async Socket Server Experiment in [HHVM][HHVM]'s [HackLang][HackLang]. it makes use of HackLang's built-in [async functions][async functions].

#### Usage

```hack
$Server = new Server('localhost', 9098);
$Server->Listen(async function(resource $Client){
  await Server::OnResponse($Client, async function(string $Data, resource $Client){
    echo "Client Sent Data: $Data\n";
  });
});
```

#### Usage.ServerOnly

When You only want the server, You can do `$Server->Listen(...)->getWaitHandle()->join()`. This is a blocking operation, which means that It won't let the program get to the next line. It'll immediately start waiting for clients to connect to it. You can also store the wait handle in a variable and join it at the end of your program, like
```hack
$WaitHandle = $Server->Listen(...)->getWaitHandle();
... Do Stuff Here
$WaitHandle->join();
```

#### Usage.ServerSidedApp

If you have a server sided app, then it of course does a lot of other stuff than just listening on an http port. In that case, You can execute two async functions and join them both, for example.
```hack
class MyServerSidedApp{
  public Server $Server;
  public function __construct(){
    $this->Server = $Server = new Server('localhost', 9098);
  }
  public async function Init():Awaitable<void>{
    // Do the regular app stuff here..
  }
}
$App = new MyServerSidedApp();
GenArrayWaitHandle::create([
  $App->Server->Listen(async function(resource $Client){
    await Server::OnResponse($Client, async function(string $Data, resource $Client){
      echo "Client Sent Data: $Data\n";
    })->getWaitHandle()
  ), $App->Init()]
)->join();
// This way your program and the HTTP Server and App will run co-currently
```

[HHVM]:https://github.com/facebook/hhvm
[HackLang]:http://hacklang.org
[async functions]:http://docs.hhvm.com/manual/en/hack.async.php