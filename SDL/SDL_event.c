#include "../common.h"
#include "SDL_event.h"

u32 sdl_to_config_map[] =
{
	SDLK_UP,
	SDLK_DOWN,
	SDLK_LEFT,
	SDLK_RIGHT,
	SDLK_LCTRL,
	SDLK_LALT,
	SDLK_LSHIFT,
	SDLK_SPACE,
	SDLK_TAB,
	SDLK_BACKSPACE,
	SDLK_RETURN,
	SDLK_ESCAPE
};

u32 key_map(u32 keys)
{
	unsigned char i, chosen_key;
	
	chosen_key = 12;

	for (i=0;i<13;i++)
	{
		if (keys == sdl_to_config_map[i])
		{
			chosen_key = config.pad[i];
			break;
		}
	}

	switch(chosen_key)
	{
		case 0:
		  return CONFIG_BUTTON_UP;

		case 1:
		  return CONFIG_BUTTON_DOWN;

		case 2:
		  return CONFIG_BUTTON_LEFT;

		case 3:
		  return CONFIG_BUTTON_RIGHT;

		case 4:
		  return CONFIG_BUTTON_I;

		case 5:
		  return CONFIG_BUTTON_II;

		case 6:
		  return CONFIG_BUTTON_III;

		case 7:
		  return CONFIG_BUTTON_IV;

		case 8:
		  return CONFIG_BUTTON_V;

		case 9:
		  return CONFIG_BUTTON_VI;

		case 10:
		  return CONFIG_BUTTON_RUN;

		case 11:
		  return CONFIG_BUTTON_SELECT;

		case 12:
		default:
		  return CONFIG_BUTTON_NONE;
	}
}

u32 joy_button_map(u32 button)
{
  switch(button)
  {
    case 2:
      return CONFIG_BUTTON_I;

    case 1:
      return CONFIG_BUTTON_II;

    case 5:
      return CONFIG_BUTTON_III;

    case 3:
      return CONFIG_BUTTON_IV;

    case 0:
      return CONFIG_BUTTON_V;

    case 4:
      return CONFIG_BUTTON_VI;

    case 9:
      return CONFIG_BUTTON_RUN;

    case 8:
      return CONFIG_BUTTON_SELECT;
  }

  return config.pad[button + 4];
}

u32 joy_hat_map(u32 hat_value)
{
  switch(hat_value)
  {
    case SDL_HAT_UP:
      return HAT_STATUS_UP;

    case SDL_HAT_RIGHTUP:
      return HAT_STATUS_UP_RIGHT;

    case SDL_HAT_RIGHT:
      return HAT_STATUS_RIGHT;

    case SDL_HAT_RIGHTDOWN:
      return HAT_STATUS_DOWN_RIGHT;

    case SDL_HAT_DOWN:
      return HAT_STATUS_DOWN;

    case SDL_HAT_LEFTDOWN:
      return HAT_STATUS_DOWN_LEFT;

    case SDL_HAT_LEFT:
      return HAT_STATUS_LEFT;

    case SDL_HAT_LEFTUP:
      return HAT_STATUS_UP_LEFT;

    default:
      return HAT_STATUS_CENTER;
  }
}

int horiz_axis = 0;
int vert_axis = 0;

u32 joy_axis_map_action(u32 axis, s32 value)
{
  if(axis & 1)
  {
    if(value < 0) vert_axis = -1;
    else if(value > 0) vert_axis = 1;
    else vert_axis = 0;
  }
  else
  {
    if(value < 0) horiz_axis = -1;
    else if(value > 0) horiz_axis = 1;
    else horiz_axis = 0;
  }
  
  if(vert_axis < 0)
  {
    if(horiz_axis < 0) return HAT_STATUS_UP_LEFT;
    else if(horiz_axis > 0) return HAT_STATUS_UP_RIGHT;
    else return HAT_STATUS_UP;
  }
  else if(vert_axis > 0)
  {
    if(horiz_axis < 0) return HAT_STATUS_DOWN_LEFT;
    else if(horiz_axis > 0) return HAT_STATUS_DOWN_RIGHT;
    else return HAT_STATUS_DOWN;
  }
  
  if(horiz_axis < 0) return HAT_STATUS_LEFT;
  if(horiz_axis > 0) return HAT_STATUS_RIGHT;
  
  return HAT_STATUS_CENTER;
}

#ifdef RS97_BUILD
int select_pressed = 0;
int start_pressed = 0;
#endif

u32 update_input(event_input_struct *event_input)
{
  SDL_Event event;

  event_input->config_button_action = CONFIG_BUTTON_NONE;
  event_input->key_action = KEY_ACTION_NONE;
  event_input->key_letter = 0;
  event_input->hat_status = HAT_STATUS_NONE;
  
  if(SDL_PollEvent(&event))
  {
#ifdef RS97_BUILD
    if(event.key.keysym.sym == SDLK_ESCAPE) select_pressed = (event.type == SDL_KEYDOWN);
    if(event.key.keysym.sym == SDLK_RETURN) start_pressed = (event.type == SDL_KEYDOWN);
    
    if(select_pressed && start_pressed) 
    {
        select_pressed = 0;
        start_pressed = 0;
        event.key.keysym.sym = SDLK_END;
    }
#endif
    switch(event.type)
    {
      case SDL_QUIT:
        event_input->action_type = INPUT_ACTION_TYPE_PRESS;
        event_input->key_action = KEY_ACTION_QUIT;
        // Deliberate fallthrough

      case SDL_KEYDOWN:
        event_input->action_type = INPUT_ACTION_TYPE_PRESS;
        event_input->key_letter = event.key.keysym.unicode;

        switch(event.key.keysym.sym)
        {
#ifdef RS97_BUILD
          case SDLK_END:
#else
          case SDLK_ESCAPE:
#endif
			event_input->config_button_action = CONFIG_BUTTON_MENU;
            //event_input->key_action = KEY_ACTION_QUIT;
            break;

          case SDLK_1:
            event_input->key_action = KEY_ACTION_BG_OFF;
            break;

          case SDLK_2:
            event_input->key_action = KEY_ACTION_SPR_OFF;
            break;

          case SDLK_F1:
            event_input->key_action = KEY_ACTION_DEBUG_BREAK;
            break;

          case SDLK_t:
            event_input->key_action = KEY_ACTION_NETPLAY_TALK;
            break;

          case SDLK_BACKQUOTE:
            event_input->config_button_action = CONFIG_BUTTON_FAST_FORWARD;
            break;

          case SDLK_F5:
            event_input->config_button_action = CONFIG_BUTTON_SAVE_STATE;
            break;

          case SDLK_F7:
            event_input->config_button_action = CONFIG_BUTTON_LOAD_STATE;
            break;

          case SDLK_m:
            event_input->config_button_action = CONFIG_BUTTON_MENU;
            break;

          case SDLK_BACKSPACE:
            event_input->key_action = KEY_ACTION_NETPLAY_TALK_CURSOR_BACKSPACE;
            event_input->config_button_action = key_map(event.key.keysym.sym);
            break;

          case SDLK_RETURN:
            event_input->key_action = KEY_ACTION_NETPLAY_TALK_CURSOR_ENTER;
            event_input->config_button_action = key_map(event.key.keysym.sym);
            break;

          case SDLK_LEFT:
            event_input->key_action = KEY_ACTION_NETPLAY_TALK_CURSOR_LEFT;
            event_input->config_button_action = key_map(event.key.keysym.sym);
            break;

          case SDLK_RIGHT:
            event_input->key_action = KEY_ACTION_NETPLAY_TALK_CURSOR_RIGHT;
            event_input->config_button_action = key_map(event.key.keysym.sym);
            break; 
     
          default:
            event_input->config_button_action = key_map(event.key.keysym.sym);
            break;
        }
        break;

      case SDL_KEYUP:
        event_input->action_type = INPUT_ACTION_TYPE_RELEASE;
        event_input->config_button_action = key_map(event.key.keysym.sym);
        break;

      case SDL_JOYBUTTONDOWN:
        event_input->action_type = INPUT_ACTION_TYPE_PRESS;
        event_input->config_button_action = joy_button_map(event.jbutton.button);
        break;

      case SDL_JOYBUTTONUP:
        event_input->action_type = INPUT_ACTION_TYPE_RELEASE;
        event_input->config_button_action = joy_button_map(event.jbutton.button);
        break;

      case SDL_JOYHATMOTION:
        event_input->hat_status = joy_hat_map(event.jhat.value);
        break;

      case SDL_JOYAXISMOTION:
        event_input->hat_status = joy_axis_map_action(event.jaxis.axis, event.jaxis.value);
        break;
    }
  }
  else
  {
    return 0;
  }

  return 1;
}

u8 gui_actions[16];

void clear_gui_actions()
{
  memset(gui_actions, 0, 16);
}

gui_action_type joy_map_gui_action(u32 button)
{
  switch(button)
  {
    case 1:
      return CURSOR_EXIT;
    case 2:
      return CURSOR_SELECT;
    case 0:
      return CURSOR_BACK;
    case 4:
      return CURSOR_PAGE_UP;
    case 5:
      return CURSOR_PAGE_DOWN;
    default:
      return CURSOR_NONE;
  }
}

gui_action_type joy_hat_map_gui_action(u32 hat_value)
{
  switch(hat_value)
  {
    case SDL_HAT_UP:
      return CURSOR_UP;

    case SDL_HAT_RIGHT:
      return CURSOR_RIGHT;

    case SDL_HAT_DOWN:
      return CURSOR_DOWN;

    case SDL_HAT_LEFT:
      return CURSOR_LEFT;

    case SDL_HAT_CENTERED:
      clear_gui_actions();
      break;
    default:
		return CURSOR_NONE;
  }
}

gui_action_type key_map_gui_action(u32 key)
{
  switch(key)
  {
    case SDLK_ESCAPE:
      return CURSOR_EXIT;

    case SDLK_DOWN:
      return CURSOR_DOWN;

    case SDLK_UP:
      return CURSOR_UP;

    case SDLK_LEFT:
      return CURSOR_LEFT;

    case SDLK_RIGHT:
      return CURSOR_RIGHT;

    case SDLK_RETURN:
    case SDLK_LCTRL:
      return CURSOR_SELECT;

    case SDLK_LALT:
      return CURSOR_BACK;

    /*case SDLK_PAGEUP:
      return CURSOR_PAGE_UP;

    case SDLK_PAGEDOWN:
      return CURSOR_PAGE_DOWN;*/
  }

  return CURSOR_NONE;
}

u32 joy_axis_map_set_gui_action(u32 axis, s32 value)
{
  if(axis & 1)
  {
    if(value < 0) 
      return CURSOR_UP;
    else
      return CURSOR_DOWN;
  }
  else
  {
    if(value < 0) 
      return CURSOR_LEFT;
    else
      return CURSOR_RIGHT;
  }
}

void get_gui_input(gui_input_struct *gui_input)
{
  SDL_Event event;
  gui_action_type gui_action = CURSOR_NONE;

  static u64 button_repeat_timestamp;
  static button_repeat_state_type button_repeat_state = BUTTON_NOT_HELD;
  static gui_action_type cursor_repeat = CURSOR_NONE;

  delay_us(100);

  while(SDL_PollEvent(&event))
  {
    switch(event.type)
    {
      case SDL_QUIT:
        quit();

      case SDL_KEYDOWN:
        gui_action = key_map_gui_action(event.key.keysym.sym);
        gui_actions[gui_action] = 1;

        if(gui_action == CURSOR_NONE)
        {
          gui_action = CURSOR_LETTER;
          gui_input->key_letter = event.key.keysym.unicode;
        }
        break;

      case SDL_KEYUP:
        gui_actions[key_map_gui_action(event.key.keysym.sym)] = 0;
        break;

      case SDL_JOYBUTTONDOWN:
        gui_action = joy_map_gui_action(event.jbutton.button);
        gui_actions[gui_action] = 1;
        break;

      case SDL_JOYBUTTONUP:
        gui_actions[joy_map_gui_action(event.jbutton.button)] = 0;
        break;

      case SDL_JOYAXISMOTION:
        if(event.jaxis.value != 0)
        {
          gui_action = 
            joy_axis_map_set_gui_action(event.jaxis.axis, event.jaxis.value);
          gui_actions[gui_action] = 1;
        }
        else
        {
            gui_actions[CURSOR_UP] = 0;
            gui_actions[CURSOR_DOWN] = 0;
            gui_actions[CURSOR_LEFT] = 0;
            gui_actions[CURSOR_RIGHT] = 0;
        }
        
        break;

      case SDL_JOYHATMOTION:
        gui_action = joy_hat_map_gui_action(event.jhat.value);
        gui_actions[gui_action] = 1;
        break;
    }
  }


  if(gui_action != CURSOR_NONE)
  {
    get_ticks_us(&button_repeat_timestamp);
    button_repeat_state = BUTTON_HELD_INITIAL;
    cursor_repeat = gui_action;
  }
  else
  {
    if(gui_actions[cursor_repeat])
    {
      u64 new_ticks;
      get_ticks_us(&new_ticks);

      if(button_repeat_state == BUTTON_HELD_INITIAL)
      {
        if((new_ticks - button_repeat_timestamp) >
         BUTTON_REPEAT_START)
        {
          gui_action = cursor_repeat;
          button_repeat_timestamp = new_ticks;
          button_repeat_state = BUTTON_HELD_REPEAT;
        }
      }

      if(button_repeat_state == BUTTON_HELD_REPEAT)
      {
        if((new_ticks - button_repeat_timestamp) >
         BUTTON_REPEAT_CONTINUE)
        {
          gui_action = cursor_repeat;
          button_repeat_timestamp = new_ticks;
        }
      }
    }
  }

  gui_input->action_type = gui_action;
}

void initialize_event()
{
	u32 i;
	u32 joystick_count = SDL_NumJoysticks();
	printf("%d joysticks\n", joystick_count);
	
	if(joystick_count > 0)
	{
		for(i=0;i<joystick_count;i++)
		{
			SDL_JoystickOpen(i);
		}
		SDL_JoystickEventState(SDL_ENABLE);
	}
}

