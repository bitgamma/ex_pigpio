#include <pigpio.h>
#include <erl_nif.h>

typedef struct _ex_pigpio_cb {
  unsigned gpio;
  ErlNifEnv *env;
  ErlNifPid receiver_pid;
  struct _ex_pigpio_cb *next;
} ex_pigpio_cb;

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
  ERL_NIF_TERM atom_bad_pulsewidth;
  ERL_NIF_TERM atom_not_servo_gpio;
  ERL_NIF_TERM atom_bad_dutyrange;

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

  ERL_NIF_TERM atom_gpio_alert;

  ex_pigpio_cb *first_cb;
} ex_pigpio_priv;

void _empty_signal_handler(int signum) {
  return;
}

void _gpio_alert_callback(int gpio, int level, uint32_t tick, void *userdata) {
  ex_pigpio_priv *priv = (ex_pigpio_priv *) userdata;
  ex_pigpio_cb *cb = priv->first_cb;

  while(cb != NULL) {
    if (cb->gpio == gpio) {
      ERL_NIF_TERM tuple = enif_make_tuple4(cb->env, priv->atom_gpio_alert, enif_make_int(cb->env, gpio), enif_make_int(cb->env, level), enif_make_uint(cb->env, tick));
      enif_send(cb->env, &cb->receiver_pid, cb->env, tuple);
      enif_clear_env(cb->env);
    }

    cb = cb->next;
  }

  return;
}

void _init_library() {
  gpioInitialise();

  int i;

  for (i = 0; i < 64; i++) {
    gpioSetSignalFunc(i, &_empty_signal_handler);
  }
}

void _terminate_library() {
  gpioTerminate();
}

static ERL_NIF_TERM set_mode(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  ex_pigpio_priv* priv;
  priv = enif_priv_data(env);

  unsigned gpio;

  if (!enif_get_uint(env, argv[0], &gpio)) {
    return enif_make_badarg(env);
  }

  unsigned mode;

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

  int err = gpioSetMode(gpio, mode);

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

  unsigned gpio;

  if (!enif_get_uint(env, argv[0], &gpio)) {
    return enif_make_badarg(env);
  }

  int mode = gpioGetMode(gpio);

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

  unsigned gpio;

  if (!enif_get_uint(env, argv[0], &gpio)) {
  	return enif_make_badarg(env);
  }

  unsigned pud;

  if (!enif_compare(argv[1], priv->atom_up)) {
    pud = PI_PUD_UP;
  } else if (!enif_compare(argv[1], priv->atom_down)) {
    pud = PI_PUD_DOWN;
  } else if (!enif_compare(argv[1], priv->atom_off)) {
    pud = PI_PUD_OFF;
  } else {
    return priv->atom_bad_pud;
  }

  int err = gpioSetPullUpDown(gpio, pud);

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

  unsigned gpio;

  if (!enif_get_uint(env, argv[0], &gpio)) {
  	return enif_make_badarg(env);
  }

  int value = gpioRead(gpio);

  if (value != PI_BAD_GPIO) {
    return enif_make_tuple2(env, priv->atom_ok, enif_make_int(env, value));
  } else {
    return enif_make_tuple2(env, priv->atom_error, priv->atom_bad_gpio);
  }
}

static ERL_NIF_TERM write(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  ex_pigpio_priv* priv;
  priv = enif_priv_data(env);

  unsigned gpio;

  if (!enif_get_uint(env, argv[0], &gpio)) {
  	return enif_make_badarg(env);
  }

  unsigned value;

  if (!enif_get_uint(env, argv[1], &value)) {
    return enif_make_badarg(env);
  }

  int err = gpioWrite(gpio, value);

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

  unsigned gpio;

  if (!enif_get_uint(env, argv[0], &gpio)) {
  	return enif_make_badarg(env);
  }

  unsigned dutycycle;

  if (!enif_get_uint(env, argv[1], &dutycycle)) {
    return enif_make_badarg(env);
  }

  int err = gpioPWM(gpio, dutycycle);

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

  unsigned gpio;

  if (!enif_get_uint(env, argv[0], &gpio)) {
  	return enif_make_badarg(env);
  }

  int value = gpioGetPWMdutycycle(gpio);

  switch(value) {
    case PI_BAD_USER_GPIO:
      return enif_make_tuple2(env, priv->atom_error, priv->atom_bad_user_gpio);
    case PI_NOT_PWM_GPIO:
      return enif_make_tuple2(env, priv->atom_error, priv->atom_not_pwm_gpio);
    default:
      return enif_make_tuple2(env, priv->atom_ok, enif_make_int(env, value));
  }
}

static ERL_NIF_TERM set_servo(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  ex_pigpio_priv* priv;
  priv = enif_priv_data(env);

  unsigned gpio;

  if (!enif_get_uint(env, argv[0], &gpio)) {
  	return enif_make_badarg(env);
  }

  unsigned pulsewidth;

  if (!enif_get_uint(env, argv[1], &pulsewidth)) {
    return enif_make_badarg(env);
  }

  int err = gpioServo(gpio, pulsewidth);

  switch(err) {
    case 0:
      return priv->atom_ok;
    case PI_BAD_USER_GPIO:
      return priv->atom_bad_user_gpio;
    case PI_BAD_PULSEWIDTH:
      return priv->atom_bad_pulsewidth;
    default:
      return priv->atom_error;
  }
}

static ERL_NIF_TERM get_servo_pulsewidth(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  ex_pigpio_priv* priv;
  priv = enif_priv_data(env);

  unsigned gpio;

  if (!enif_get_uint(env, argv[0], &gpio)) {
  	return enif_make_badarg(env);
  }

  int value = gpioGetServoPulsewidth(gpio);

  switch(value) {
    case PI_BAD_USER_GPIO:
      return enif_make_tuple2(env, priv->atom_error, priv->atom_bad_user_gpio);
    case PI_NOT_SERVO_GPIO:
      return enif_make_tuple2(env, priv->atom_error, priv->atom_not_servo_gpio);
    default:
      return enif_make_tuple2(env, priv->atom_ok, enif_make_int(env, value));
  }
}

static ERL_NIF_TERM udelay(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  uint32_t usec;

  if (!enif_get_uint(env, argv[0], &usec)) {
  	return enif_make_badarg(env);
  }

  uint32_t value = gpioDelay(usec);

  return enif_make_uint(env, value);
}

static ERL_NIF_TERM add_alert(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  ex_pigpio_priv* priv;
  priv = enif_priv_data(env);

  unsigned gpio;

  if (!enif_get_uint(env, argv[0], &gpio)) {
  	return enif_make_badarg(env);
  }

  if (!enif_is_pid(env, argv[1])) {
    return enif_make_badarg(env);
  }

  int err = gpioSetAlertFuncEx(gpio, &_gpio_alert_callback, priv);

  ex_pigpio_cb *callback;

  switch(err) {
    case 0:
      callback = enif_alloc(sizeof(ex_pigpio_cb));
      callback->gpio = gpio;
      callback->env = enif_alloc_env();
      callback->next = priv->first_cb;
      enif_get_local_pid(env, argv[1], &callback->receiver_pid);
      priv->first_cb = callback;
      return priv->atom_ok;
    case PI_BAD_USER_GPIO:
      return priv->atom_bad_user_gpio;
    default:
      return priv->atom_error;
  }
}

static ERL_NIF_TERM remove_alert(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  ex_pigpio_priv* priv;
  priv = enif_priv_data(env);

  unsigned gpio;

  if (!enif_get_uint(env, argv[0], &gpio)) {
    return enif_make_badarg(env);
  }

  ERL_NIF_TERM pid_to_remove = argv[1];

  if (!enif_is_pid(env, pid_to_remove)) {
    return enif_make_badarg(env);
  }

  int is_last_for_gpio = 1;
  int found = 0;

  ex_pigpio_cb *callback = priv->first_cb;
  ex_pigpio_cb *prev_callback = NULL;

  while(callback != NULL) {
    ex_pigpio_cb *to_remove = NULL;

    if (callback->gpio == gpio) {
      ERL_NIF_TERM last_pid = enif_make_pid(env, &callback->receiver_pid);

      if (!enif_compare(pid_to_remove, last_pid)) {
        if (prev_callback == NULL) {
          priv->first_cb = callback->next;
        } else {
          prev_callback->next = callback->next;
        }

        found = 1;
        to_remove = callback;
      } else {
        is_last_for_gpio = 0;
      }
    }

    callback = callback->next;

    if (to_remove != NULL) {
      enif_free_env(to_remove->env);
      enif_free(to_remove);
    } else {
      prev_callback = callback;
    }
  }

  if (is_last_for_gpio && found) {
    gpioSetAlertFuncEx(gpio, NULL, NULL);
  }

  return priv->atom_ok;
}

static ERL_NIF_TERM set_pwm_range(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  ex_pigpio_priv* priv;
  priv = enif_priv_data(env);

  unsigned gpio;

  if (!enif_get_uint(env, argv[0], &gpio)) {
  	return enif_make_badarg(env);
  }

  unsigned range;

  if (!enif_get_uint(env, argv[1], &range)) {
    return enif_make_badarg(env);
  }

  int err = gpioSetPWMrange(gpio, range);

  switch(err) {
    case 0:
      return priv->atom_ok;
    case PI_BAD_USER_GPIO:
      return priv->atom_bad_user_gpio;
    case PI_BAD_DUTYRANGE:
      return priv->atom_bad_dutyrange;
    default:
      return priv->atom_error;
  }
}

static ERL_NIF_TERM get_pwm_range(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  ex_pigpio_priv* priv;
  priv = enif_priv_data(env);

  unsigned gpio;

  if (!enif_get_uint(env, argv[0], &gpio)) {
  	return enif_make_badarg(env);
  }

  int value = gpioGetPWMrange(gpio);

  switch(value) {
    case PI_BAD_USER_GPIO:
      return enif_make_tuple2(env, priv->atom_error, priv->atom_bad_user_gpio);
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
  { "set_servo", 2, set_servo },
  { "get_servo_pulsewidth", 1, get_servo_pulsewidth },
  { "udelay", 1, udelay },
  { "add_alert", 2, add_alert },
  { "remove_alert", 2, remove_alert },
  { "set_pwm_range", 2, set_pwm_range },
  { "get_pwm_range", 1, get_pwm_range }
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
  data->atom_bad_pulsewidth = enif_make_atom(env, "bad_pulsewidth");
  data->atom_not_servo_gpio = enif_make_atom(env, "not_servo_gpio");
  data->atom_bad_dutyrange = enif_make_atom(env, "bad_dutyrange");

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

  data->atom_gpio_alert = enif_make_atom(env, "gpio_alert");

  data->first_cb = NULL;

  *priv = (void*) data;

  _init_library();

  return 0;
}

static int reload(ErlNifEnv* env, void** priv, ERL_NIF_TERM info) {
  _terminate_library();
  _init_library();

  return 0;
}

static int upgrade(ErlNifEnv* env, void** priv, void** old_priv, ERL_NIF_TERM info) {
  _terminate_library();
  return load(env, priv, info);
}

static void unload(ErlNifEnv* env, void* priv) {
  _terminate_library();

  ex_pigpio_priv* data = (ex_pigpio_priv*) priv;

  while(data->first_cb != NULL) {
    ex_pigpio_cb *next = data->first_cb->next;

    enif_free_env(data->first_cb->env);
    enif_free(data->first_cb);
    data->first_cb = next;
  }

  enif_free(priv);
}

ERL_NIF_INIT(Elixir.ExPigpio, funcs, &load, &reload, &upgrade, &unload)
