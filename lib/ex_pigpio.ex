defmodule ExPigpio do
  @on_load :init

  def init do
    path = Application.app_dir(:ex_pigpio, "priv/ex_pigpio") |> String.to_char_list
    :ok = :erlang.load_nif(path, 0)
  end

  def set_mode(_pin, _mode) do
    exit(:nif_not_loaded)
  end

  def get_mode(_pin) do
    exit(:nif_not_loaded)
  end

  def set_pull_resistor(_pin, _pud) do
    exit(:nif_not_loaded)
  end

  def read(_pin) do
    exit(:nif_not_loaded)
  end

  def write(_pin, _value) do
    exit(:nif_not_loaded)
  end

  def set_pwm(_pin, _dutycycle) do
    exit(:nif_not_loaded)
  end

  def get_pwm_dutycycle(_pin) do
    exit(:nif_not_loaded)
  end

  def set_servo(_pin, _pulsewidth) do
    exit(:nif_not_loaded)
  end

  def get_servo_pulsewidth(_pin) do
    exit(:nif_not_loaded)
  end

  def udelay(_usec) do
    exit(:nif_not_loaded)
  end
end
