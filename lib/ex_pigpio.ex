defmodule ExPigpio do
  @on_load :init

  def init do
    path = Application.app_dir(:ex_pigpio, "priv/ex_pigpio") |> String.to_char_list
    :ok = :erlang.load_nif(path, 0)
  end

  def set_mode(_gpio, _mode) do
    exit(:nif_not_loaded)
  end

  def get_mode(_gpio) do
    exit(:nif_not_loaded)
  end

  def set_pull_resistor(_gpio, _pud) do
    exit(:nif_not_loaded)
  end

  def read(_gpio) do
    exit(:nif_not_loaded)
  end

  def write(_gpio, _value) do
    exit(:nif_not_loaded)
  end

  def set_pwm(_gpio, _dutycycle) do
    exit(:nif_not_loaded)
  end

  def get_pwm_dutycycle(_gpio) do
    exit(:nif_not_loaded)
  end

  def set_servo(_gpio, _pulsewidth) do
    exit(:nif_not_loaded)
  end

  def get_servo_pulsewidth(_gpio) do
    exit(:nif_not_loaded)
  end

  def udelay(_usec) do
    exit(:nif_not_loaded)
  end

  def add_alert(_gpio, _pid) do
    exit(:nif_not_loaded)
  end

  def remove_alert(_gpio, _pid) do
    exit(:nif_not_loaded)
  end
end
