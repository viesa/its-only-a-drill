#include "Event.h"
#include "Input.h"

struct Event
{
    SDL_Event m_events;
    Input *m_input;
};

Event *EventCreate(Input *input)
{
    Event *event_ret = (Event *)SDL_malloc(sizeof(Event));
    event_ret->m_input = input;
    return event_ret;
}
void EventDestroy(Event *event)
{
    SDL_free(event);
}

void EventPollAll(Event *event)
{
    SDL_bool _true = SDL_TRUE;
    SDL_bool _false = SDL_FALSE;
    while (SDL_PollEvent(&event->m_events))
    {
        switch (event->m_events.type)
        {
        case SDL_QUIT:
            InputSet(event->m_input, KEY_ESC, _true);
            break;

        case SDL_KEYDOWN:
        {
            switch (event->m_events.key.keysym.sym)
            {
            case SDLK_a:
                InputSet(event->m_input, KEY_A, _true);
                break;
            case SDLK_b:
                InputSet(event->m_input, KEY_B, _true);
                break;
            case SDLK_c:
                InputSet(event->m_input, KEY_C, _true);
                break;
            case SDLK_d:
                InputSet(event->m_input, KEY_D, _true);
                break;
            case SDLK_e:
                InputSet(event->m_input, KEY_E, _true);
                break;
            case SDLK_f:
                InputSet(event->m_input, KEY_F, _true);
                break;
            case SDLK_g:
                InputSet(event->m_input, KEY_G, _true);
                break;
            case SDLK_h:
                InputSet(event->m_input, KEY_H, _true);
                break;
            case SDLK_i:
                InputSet(event->m_input, KEY_I, _true);
                break;
            case SDLK_j:
                InputSet(event->m_input, KEY_J, _true);
                break;
            case SDLK_k:
                InputSet(event->m_input, KEY_K, _true);
                break;
            case SDLK_l:
                InputSet(event->m_input, KEY_L, _true);
                break;
            case SDLK_m:
                InputSet(event->m_input, KEY_M, _true);
                break;
            case SDLK_n:
                InputSet(event->m_input, KEY_N, _true);
                break;
            case SDLK_o:
                InputSet(event->m_input, KEY_O, _true);
                break;
            case SDLK_p:
                InputSet(event->m_input, KEY_P, _true);
                break;
            case SDLK_q:
                InputSet(event->m_input, KEY_Q, _true);
                break;
            case SDLK_r:
                InputSet(event->m_input, KEY_R, _true);
                break;
            case SDLK_s:
                InputSet(event->m_input, KEY_S, _true);
                break;
            case SDLK_t:
                InputSet(event->m_input, KEY_T, _true);
                break;
            case SDLK_u:
                InputSet(event->m_input, KEY_U, _true);
                break;
            case SDLK_v:
                InputSet(event->m_input, KEY_V, _true);
                break;
            case SDLK_w:
                InputSet(event->m_input, KEY_W, _true);
                break;
            case SDLK_x:
                InputSet(event->m_input, KEY_X, _true);
                break;
            case SDLK_y:
                InputSet(event->m_input, KEY_Y, _true);
                break;
            case SDLK_z:
                InputSet(event->m_input, KEY_Z, _true);
                break;
            case SDLK_ESCAPE:
                InputSet(event->m_input, KEY_ESC, _true);
                break;
            default:
                break;
            }
            break;
        }
        case SDL_KEYUP:
        {
            switch (event->m_events.key.keysym.sym)
            {
            case SDLK_a:
                InputSet(event->m_input, KEY_A, _false);
                break;
            case SDLK_b:
                InputSet(event->m_input, KEY_B, _false);
                break;
            case SDLK_c:
                InputSet(event->m_input, KEY_C, _false);
                break;
            case SDLK_d:
                InputSet(event->m_input, KEY_D, _false);
                break;
            case SDLK_e:
                InputSet(event->m_input, KEY_E, _false);
                break;
            case SDLK_f:
                InputSet(event->m_input, KEY_F, _false);
                break;
            case SDLK_g:
                InputSet(event->m_input, KEY_G, _false);
                break;
            case SDLK_h:
                InputSet(event->m_input, KEY_H, _false);
                break;
            case SDLK_i:
                InputSet(event->m_input, KEY_I, _false);
                break;
            case SDLK_j:
                InputSet(event->m_input, KEY_J, _false);
                break;
            case SDLK_k:
                InputSet(event->m_input, KEY_K, _false);
                break;
            case SDLK_l:
                InputSet(event->m_input, KEY_L, _false);
                break;
            case SDLK_m:
                InputSet(event->m_input, KEY_M, _false);
                break;
            case SDLK_n:
                InputSet(event->m_input, KEY_N, _false);
                break;
            case SDLK_o:
                InputSet(event->m_input, KEY_O, _false);
                break;
            case SDLK_p:
                InputSet(event->m_input, KEY_P, _false);
                break;
            case SDLK_q:
                InputSet(event->m_input, KEY_Q, _false);
                break;
            case SDLK_r:
                InputSet(event->m_input, KEY_R, _false);
                break;
            case SDLK_s:
                InputSet(event->m_input, KEY_S, _false);
                break;
            case SDLK_t:
                InputSet(event->m_input, KEY_T, _false);
                break;
            case SDLK_u:
                InputSet(event->m_input, KEY_U, _false);
                break;
            case SDLK_v:
                InputSet(event->m_input, KEY_V, _false);
                break;
            case SDLK_w:
                InputSet(event->m_input, KEY_W, _false);
                break;
            case SDLK_x:
                InputSet(event->m_input, KEY_X, _false);
                break;
            case SDLK_y:
                InputSet(event->m_input, KEY_Y, _false);
                break;
            case SDLK_z:
                InputSet(event->m_input, KEY_Z, _false);
                break;
            case SDLK_ESCAPE:
                InputSet(event->m_input, KEY_ESC, _false);
                break;
            default:
                break;
            }
        }
        default:
            break;
        }
    }
}