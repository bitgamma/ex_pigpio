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

  @doc """
  Starts PWM on the given `gpio` pin. Set the dutycycle between 0 (off) and range (fully on). Range defaults to 255.

  This function returns:
    * `:ok` - PWM dutycycle for the `gpio` pin
    * `:bad_user_gpio` - the given `user gpio` pin is invalid
    * `:bad_dutycycle` - the given `dutycycle` is invalid
    * `:error` - unknown error
  """
  def set_pwm(_gpio, _dutycycle) do
    exit(:nif_not_loaded)
  end

  @doc """
  Gets the `dutycycle` setting of the given `gpio` pin.

  This function returns:
    * `{:error, :bad_user_gpio}` - the given `user gpio` pin is invalid
    * `{:error, :not_pwm_gpio}` - the given `gpio` pin is not PWM
    * `{:ok, value}` - the PWM dutycycle setting for the `gpio` pin
  """
  def get_pwm_dutycycle(_gpio) do
    exit(:nif_not_loaded)
  end

  @doc """
  Sets the `dutycycle range` to be used for the given `gpio` pin. Subsequent calls to gpioPWM will use a dutycycle between 0 (off) and range (fully on).

  This function returns:
    * `{:error, :bad_user_gpio}` - the given `user gpio` pin is invalid
    * `{:error, :bad_dutyrange}` - the given `dutyrange` is invalid
    * `{:ok, value}` - the real range for the gpio's pin frequency
  """
  def set_pwm_range(_gpio, _range) do
    exit(:nif_not_loaded)
  end

  @doc """
  Gets the `dutycycle` range used for the given `gpio` pin.

  This function returns:
    * `{:error, :bad_user_gpio}` - the given `user gpio` pin is invalid
    * `{:ok, value}` - the dutycycle range used for the `gpio` pin
  """
  def get_pwm_range(_gpio) do
    exit(:nif_not_loaded)
  end

  @doc """
  Starts servo pulses on the given `gpio` pin, 0 (off), 500 (most anti-clockwise) to 2500 (most clockwise).

  This function returns:
    * `:ok` - servo pulsewidth for the `gpio` pin
    * `:bad_user_gpio` - the given `user gpio` pin is invalid
    * `:bad_pulsewidth` - the given `pulsewidth` is invalid
    * `:error` - unknown error
  """
  def set_servo(_gpio, _pulsewidth) do
    exit(:nif_not_loaded)
  end

  @doc """
  Gets the servo `pulsewidth` setting for the given `gpio` pin.

  This function returns:
    * `{:error, :bad_user_gpio}` - the given `user gpio` pin is invalid
    * `{:error, :not_servo_gpio}` - the given `gpio` pin is not a servo
    * `{:ok, value}` - the servo pulsewidth setting for the `gpio` pin
  """
  def get_servo_pulsewidth(_gpio) do
    exit(:nif_not_loaded)
  end

  @doc """
  Delays the given `gpio` pin for at least the number of microseconds specified by micros.

  This function returns:
    * `value` - the actual length of the delay in microseconds for the `gpio` pin
  """
  def udelay(_usec) do
    exit(:nif_not_loaded)
  end

  @doc """
  Registers a function to be called (a callback) when the the given `gpio` pin changes state.

  This function returns:
    * `:ok` - a callback on the `gpio` pin state change
    * `:bad_user_gpio` - the given `user gpio` pin is invalid
  """
  def add_alert(_gpio, _pid) do
    exit(:nif_not_loaded)
  end

  @doc """
  Removes alert on the the given `gpio` pin state change, passing NULL as the function.

  This function returns:
    * `:ok` - a callback removed
  """
  def remove_alert(_gpio, _pid) do
    exit(:nif_not_loaded)
  end

end
