defmodule ExPigpio do
  @moduledoc """
  ELixir NIF for the PI GPIO library found at github: joan2937/pigpio.
  """
  @on_load :init

  @doc """
  Loads the NIF and initializes the library.
  """
  def init do
    path = Application.app_dir(:ex_pigpio, "priv/ex_pigpio") |> String.to_char_list
    :ok = :erlang.load_nif(path, 0)
  end

  @doc """
  Sets the `mode` of the given `gpio` pin.

  The allowed modes are:
    * `:input`
    * `:output`
    * `:alt0`
    * `:alt1`
    * `:alt2`
    * `:alt3`
    * `:alt4`
    * `:alt5`

  This function returns:
    * `:ok` - the mode of the `gpio` pin has been changed
    * `:bad_gpio` - the given `gpio` pin is invalid
    * `:bad_mode` - the given `mode` is undefined
    * `:error` - unknown error
  """
  def set_mode(_gpio, _mode) do
    exit(:nif_not_loaded)
  end

  @doc """
  Gets the `mode` of the given `gpio` pin. See `set_mode` for a list of possible modes.

  This function returns:
    * `{:ok, mode}` - the mode of the `gpio` pin
    * `{:error, :bad_gpio}` - the given `gpio` pin is invalid
    * `{:error, :error}` - unknown error
  """
  def get_mode(_gpio) do
    exit(:nif_not_loaded)
  end

  @doc """
  Sets the pull up/down resistor on the given `gpio` pin to the given `pud` value.

  The values of `pud` can be:
    * `:up` - pullup resistor
    * `:down` - pulldown resistor
    * `:off` - pull resistor off

  This function returns:
    * `:ok` - the pull up/down of the `gpio` pin has been set
    * `:bad_gpio` - the given `gpio` pin is invalid
    * `:bad_pud` - the given `pud` is invalid
    * `:error` - unknown error
  """
  def set_pull_resistor(_gpio, _pud) do
    exit(:nif_not_loaded)
  end

  @doc """
  Read the value of the given `gpio` pin. Can be 1 or 0.

  This function returns:
    * `{:ok, level}` - the level of the `gpio` pin
    * `{:error, :bad_gpio}` - the given `gpio` pin is invalid
    * `{:error, :error}` - unknown error
  """
  def read(_gpio) do
    exit(:nif_not_loaded)
  end

  @doc """
  Sets the level of the given `gpio` pin. Can be 1 or 0.

  This function returns:
    * `:ok` - the mode of the `gpio` pin
    * `:bad_gpio` - the given `gpio` pin is invalid
    * `:bad_level` - the given `level` is invalid
    * `:error` - unknown error
  """
  def write(_gpio, _level) do
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

  def set_pwm_range(_gpio, _range) do
    exit(:nif_not_loaded)
  end

  def get_pwm_range(_gpio) do
    exit(:nif_not_loaded)
  end
end
