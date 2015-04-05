#include <pigpio.h>
#include <erl_nif.h>

typedef struct {
  ERL_NIF_TERM atom_ok;
  ERL_NIF_TERM atom_error;
  ERL_NIF_TERM atom_bad_gpio;
  ERL_NIF_TERM atom_bad_level;
  ERL_NIF_TERM atom_input;
  ERL_NIF_TERM atom_output;
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
  } else {
    return enif_make_badarg(env);
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

static ERL_NIF_TERM read(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  ex_pigpio_priv* priv;
  priv = enif_priv_data(env);

  int pin;

  if (!enif_get_int(env, argv[0], &pin)) {
  	return enif_make_badarg(env);
  }

  int value = gpioRead(pin);

  if (value != PI_BAD_GPIO) {
    return enif_make_tuple2(env, priv->atom_ok, enif_make_int(env, value))
  } else {
    return enif_make_tuple2(env, priv->atom_error, priv->atom_bad_gpio)
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

static ErlNifFunc funcs[] = {
  { "set_mode", 2, set_mode },
  { "read", 1, read },
  { "write", 2, write },
};

static int load(ErlNifEnv* env, void** priv, ERL_NIF_TERM info) {
  ex_pigpio_priv* data = enif_alloc(sizeof(ex_pigpio_priv));

  if (data == NULL) {
    return 1;
  }

  data->atom_ok = enif_make_atom(env, "ok");
  data->atom_error = enif_make_atom(env, "error");
  data->atom_bad_gpio = enif_make_atom(env, "bad_gpio");
  data->atom_bad_level = enif_make_atom(env, "bad_level");
  data->atom_error = enif_make_atom(env, "error");
  data->atom_input = enif_make_atom(env, "input");
  data->atom_output = enif_make_atom(env, "output");

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
