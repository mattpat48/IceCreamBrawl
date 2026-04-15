#pragma once

// Evento per mettere in pausa/riprendere il gioco
struct PauseToggleEvent { mutable bool handled = false; };
struct PlayerDeathEvent { mutable bool handled = false; };
struct PlayerRespawnEvent { mutable bool handled = false; };