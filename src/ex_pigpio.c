#include <pigpio.h>
#include <erl_nif.h>

typedef struct {
  ERL_NIF_TERM atom_ok;
  ERL_NIF_TERM atom_error;

  ERL_NIF_TERM atom_bad_gpio;
  ERL_NIF_TERM atom_bad_user_gpio;
  ERL_NIF_TERM atom_bad_level;
  ERL_NIF_TERM atom_bad_mode;
  ERL_NIF_TERM atom_bad_pud;
  ERL_NIF_TERM atom_bad_dutycycle;
  ERL_NIF_TERM atom_not_pwm_gpio;

  ERL_NIF_TERM atom_input;
  ERL_NIF_TERM atom_output;
  ERL_NIF_TERM atom_alt0;
  ERL_NIF_TERM atom_alt1;
  ERL_NIF_TERM atom_alt2;
  ERL_NIF_TERM atom_alt3;
  ERL_NIF_TERM atom_alt4;
  ERL_NIF_TERM atom_alt5;

  ERL_NIF_TERM atom_up;
  ERL_NIF_TERM atom_down;
  ERL_NIF_TERM atom_off;
} ex_pigpio_priv;

static ERL_NIF_TERM set_mode(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  ex_pigpio_priv* priv;
  priv = enif_priv_data(env);

  int pin;

  if (!enif_get_int(env, argv[0], &pin)) {
    return enif_make_badarg(env);
  }

  int mode;

  if (!enif_compare(argv[1], priv->atom_input)) {
    mode = PI_INPUT;
  } else if (!enif_compare(argv[1], priv->atom_output)) {
    mode = PI_OUTPUT;
  } else if (!enif_compare(argv[1], priv->atom_alt0)) {
    mode = PI_ALT0;
  } else if (!enif_compare(argv[1], priv->atom_alt1)) {
    mode = PI_ALT1;
  } else if (!enif_compare(argv[1], priv->atom_alt2)) {
    mode = PI_ALT2;
  } else if (!enif_compare(argv[1], priv->atom_alt3)) {
    mode = PI_ALT3;
  } else if (!enif_compare(argv[1], priv->atom_alt4)) {
    mode = PI_ALT4;
  } else if (!enif_compare(argv[1], priv->atom_alt5)) {
    mode = PI_ALT5;
  } else {
    return priv->atom_bad_mode;
  }

  int err = gpioSetMode(pin, mode);

  switch(err) {
    case 0:
      return priv->atom_ok;
    case PI_BAD_GPIO:
      return priv->atom_bad_gpio;
    default:
      return priv->atom_error;
  }
}

static ERL_NIF_TERM get_mode(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  ex_pigpio_priv* priv;
  priv = enif_priv_data(env);

  int pin;

  if (!enif_get_int(env, argv[0], &pin)) {
    return enif_make_badarg(env);
  }

  int mode = gpioGetMode(pin);

  switch(mode) {
    case PI_INPUT:
      return enif_make_tuple2(env, priv->atom_ok, priv->atom_input);
    case PI_OUTPUT:
      return enif_make_tuple2(env, priv->atom_ok, priv->atom_output);
    case PI_ALT0:
      return enif_make_tuple2(env, priv->atom_ok, priv->atom_alt0);
    case PI_ALT1:
      return enif_make_tuple2(env, priv->atom_ok, priv->atom_alt1);
    case PI_ALT2:
      return enif_make_tuple2(env, priv->atom_ok, priv->atom_alt2);
    case PI_ALT3:
      return enif_make_tuple2(env, priv->atom_ok, priv->atom_alt3);
    case PI_ALT4:
      return enif_make_tuple2(env, priv->atom_ok, priv->atom_alt4);
    case PI_ALT5:
      return enif_make_tuple2(env, priv->atom_ok, priv->atom_alt5);
    case PI_BAD_GPIO:
      return enif_make_tuple2(env, priv->atom_error, priv->atom_bad_gpio);
    default:
      return enif_make_tuple2(env, priv->atom_error, priv->atom_error);
  }
}

static ERL_NIF_TERM set_pull_resistor(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  ex_pigpio_priv* priv;
  priv = enif_priv_data(env);

  int pin;

  if (!enif_get_int(env, argv[0], &pin)) {
  	return enif_make_badarg(env);
  }

  int pud;

  if (!enif_compare(argv[1], priv->atom_up)) {
    pud = PI_PUD_UP;
  } else if (!enif_compare(argv[1], priv->atom_down)) {
    pud = PI_PUD_DOWN;
  } else if (!enif_compare(argv[1], priv->atom_off)) {
    pud = PI_PUD_OFF;
  } else {
    return priv->atom_bad_pud;
  }

  int err = gpioSetPullUpDown(pin, pud);

  switch(err) {
    case 0:
      return priv->atom_ok;
    case PI_BAD_GPIO:
      return priv->atom_bad_gpio;
    case PI_BAD_PUD:
      return priv->atom_bad_pud;
    default:
      return priv->atom_error;
  }
}

static ERL_NIF_TERM read(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  ex_pigpio_priv* priv;
  priv = enif_priv_data(env);

  int pin;

  if (!enif_get_int(env, argv[0], &pin)) {
  	return enif_make_badarg(env);
  }

  int value = gpioRead(pin);

  if (value != PI_BAD_GPIO) {
    return enif_make_tuple2(env, priv->atom_ok, enif_make_int(env, value));
  } else {
    return enif_make_tuple2(env, priv->atom_error, priv->atom_bad_gpio);
  }
}

static ERL_NIF_TERM write(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  ex_pigpio_priv* priv;
  priv = enif_priv_data(env);

  int pin;

  if (!enif_get_int(env, argv[0], &pin)) {
  	return enif_make_badarg(env);
  }

  int value;

  if (!enif_get_int(env, argv[1], &value)) {
    return enif_make_badarg(env);
  }

  int err = gpioWrite(pin, value);

  switch(err) {
    case 0:
      return priv->atom_ok;
    case PI_BAD_GPIO:
      return priv->atom_bad_gpio;
    case PI_BAD_LEVEL:
      return priv->atom_bad_level;
    default:
      return priv->atom_error;
  }
}

static ERL_NIF_TERM set_pwm(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  ex_pigpio_priv* priv;
  priv = enif_priv_data(env);

  int pin;

  if (!enif_get_int(env, argv[0], &pin)) {
  	return enif_make_badarg(env);
  }

  int dutycycle;

  if (!enif_get_int(env, argv[1], &dutycycle)) {
    return enif_make_badarg(env);
  }

  int err = gpioPWM(pin, dutycycle);

  switch(err) {
    case 0:
      return priv->atom_ok;
    case PI_BAD_USER_GPIO:
      return priv->atom_bad_user_gpio;
    case PI_BAD_DUTYCYCLE:
      return priv->atom_bad_dutycycle;
    default:
      return priv->atom_error;
  }
}

static ERL_NIF_TERM get_pwm_dutycycle(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  ex_pigpio_priv* priv;
  priv = enif_priv_data(env);

  int pin;

  if (!enif_get_int(env, argv[0], &pin)) {
  	return enif_make_badarg(env);
  }

  int value = gpioGetPWMdutycycle(pin);

  switch(value) {
    case PI_BAD_USER_GPIO:
      return enif_make_tuple2(env, priv->atom_error, priv->atom_bad_user_gpio);
    case PI_NOT_PWM_GPIO:
      return enif_make_tuple2(env, priv->atom_error, priv->atom_not_pwm_gpio);
    default:
      return enif_make_tuple2(env, priv->atom_ok, enif_make_int(env, value));
  }
}


static ErlNifFunc funcs[] = {
  { "set_mode", 2, set_mode },
  { "get_mode", 1, get_mode },
  { "set_pull_resistor", 2, set_pull_resistor },
  { "read", 1, read },
  { "write", 2, write },
  { "set_pwm", 2, set_pwm },
  { "get_pwm_dutycycle", 1, get_pwm_dutycycle },
};

static int load(ErlNifEnv* env, void** priv, ERL_NIF_TERM info) {
  ex_pigpio_priv* data = enif_alloc(sizeof(ex_pigpio_priv));

  if (data == NULL) {
    return 1;
  }

  data->atom_ok = enif_make_atom(env, "ok");
  data->atom_error = enif_make_atom(env, "error");
  data->atom_bad_gpio = enif_make_atom(env, "bad_gpio");
  data->atom_bad_user_gpio = enif_make_atom(env, "bad_user_gpio");
  data->atom_bad_level = enif_make_atom(env, "bad_level");
  data->atom_bad_mode = enif_make_atom(env, "bad_mode");
  data->atom_bad_pud = enif_make_atom(env, "bad_pud");
  data->atom_bad_dutycycle = enif_make_atom(env, "bad_dutycycle");
  data->atom_not_pwm_gpio = enif_make_atom(env, "not_pwm_gpio");

  data->atom_input = enif_make_atom(env, "input");
  data->atom_output = enif_make_atom(env, "output");
  data->atom_alt0 = enif_make_atom(env, "alt0");
  data->atom_alt1 = enif_make_atom(env, "alt1");
  data->atom_alt2 = enif_make_atom(env, "alt2");
  data->atom_alt3 = enif_make_atom(env, "alt3");
  data->atom_alt4 = enif_make_atom(env, "alt4");
  data->atom_alt5 = enif_make_atom(env, "alt5");

  data->atom_up = enif_make_atom(env, "up");
  data->atom_down = enif_make_atom(env, "down");
  data->atom_off = enif_make_atom(env, "off");

  *priv = (void*) data;

  gpioInitialise();

  return 0;
}

static int reload(ErlNifEnv* env, void** priv, ERL_NIF_TERM info) {
  gpioTerminate();
  gpioInitialise();

  return 0;
}

static int upgrade(ErlNifEnv* env, void** priv, void** old_priv, ERL_NIF_TERM info) {
  gpioTerminate();
  return load(env, priv, info);
}

static void unload(ErlNifEnv* env, void* priv) {
  gpioTerminate();
  enif_free(priv);
}

ERL_NIF_INIT(Elixir.ExPigpio, funcs, &load, &reload, &upgrade, &unload)
