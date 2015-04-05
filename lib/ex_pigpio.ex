defmodule ExPigpio do
  @on_load :init

  def init do
    path = Application.app_dir(:ex_pigpio, "priv/ex_pigpio") |> String.to_char_list
    :ok = :erlang.load_nif(path, 0)
  end

  def set_mode(pin, mode) do
    exit(:nif_not_loaded)
  end

  def read(pin) do
    exit(:nif_not_loaded)
  end

  def write(pin, value) do
    exit(:nif_not_loaded)
  end
end
