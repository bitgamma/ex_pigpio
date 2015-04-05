
defmodule Mix.Tasks.Compile.Pigpio do
  @shortdoc "Compiles Pigpio"

  def run(_) do
    {result, _error_code} = System.cmd("make", ["priv/ex_pigpio.so"], stderr_to_stdout: true)
    Mix.shell.info result
    :ok
  end
end

defmodule ExPigpio.Mixfile do
  use Mix.Project

  def project do
    [app: :ex_pigpio,
     version: "0.0.1",
     elixir: "~> 1.0",
     build_embedded: Mix.env == :prod,
     start_permanent: Mix.env == :prod,
     compilers: [:pigpio, :elixir, :app],
     deps: deps]
  end

  def application do
    []
  end

  defp deps do
    [{:pigpio, github: "joan2937/pigpio", app: false}]
  end
end
