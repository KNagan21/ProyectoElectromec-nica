#pragma once
// Pagina web del tenidor: la sirve el ESP32 en "/" (ver conectividad.cpp).
static const char PAGINA[] = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1, viewport-fit=cover">
<title>Teñidor automático</title>
<style>
:root{--bg:#eef0f6;--card:#fff;--ink:#16162b;--mut:#5d5f7a;--line:#d6d8e6;--acc:#4338a8;--on:#fff;--bad:#b42318;
  box-sizing:border-box;padding-top:env(safe-area-inset-top,0px);padding-bottom:env(safe-area-inset-bottom,0px)}
@media(prefers-color-scheme:dark){:root:not([data-theme="light"]){--bg:#13121e;--card:#1d1c2d;--ink:#ebe9f7;--mut:#a09db9;--line:#323049;--acc:#a9a4ff;--on:#17143a;--bad:#f87171}}
:root[data-theme="dark"]{--bg:#13121e;--card:#1d1c2d;--ink:#ebe9f7;--mut:#a09db9;--line:#323049;--acc:#a9a4ff;--on:#17143a;--bad:#f87171}
html{scroll-padding-top:env(safe-area-inset-top,0px)}
*{box-sizing:border-box}
body{margin:0;background:var(--bg);color:var(--ink);font:16px/1.45 system-ui,-apple-system,"Segoe UI",Roboto,sans-serif}
.wrap{max-width:560px;margin:0 auto;padding:16px 16px 40px}
h1{font-size:1.4rem;margin:4px 0 14px;letter-spacing:-.01em}
h2{font-size:1.05rem;margin:0 0 4px}
p{margin:0 0 10px;color:var(--mut);font-size:.92rem}
button{font:inherit;min-height:44px;border-radius:10px;border:1px solid var(--line);background:var(--card);color:var(--ink);padding:0 14px;cursor:pointer}
button:focus-visible,input:focus-visible{outline:3px solid var(--acc);outline-offset:2px}
button:disabled{opacity:.4;cursor:not-allowed}
.pri{background:var(--acc);color:var(--on);border-color:var(--acc);font-weight:600}
.sm{min-height:36px;padding:0 10px;font-size:.9rem}
.tabs{display:grid;grid-template-columns:1fr 1fr;gap:4px;background:var(--line);padding:4px;border-radius:12px;margin-bottom:16px}
.tabs button{border:0;background:transparent;color:var(--mut);font-weight:600;border-radius:9px}
.tabs button[aria-selected=true]{background:var(--card);color:var(--ink)}
.card{background:var(--card);border:1px solid var(--line);border-radius:14px;padding:14px;margin-bottom:12px}
.demo{background:var(--card);border:1px dashed var(--acc);border-radius:10px;padding:8px 12px;color:var(--ink)}
.top{display:flex;justify-content:space-between;align-items:center;margin-bottom:10px}
.row{display:flex;flex-wrap:wrap;gap:8px;margin-bottom:12px}
.seg-row{display:grid;grid-template-columns:repeat(2,1fr);gap:6px}
.seg{display:flex;align-items:center;gap:8px;padding:6px 10px;text-align:left;line-height:1.2;font-weight:700;font-size:1.05rem}
.seg small{font-weight:500;font-size:.85rem}
.seg[aria-pressed=true]{background:var(--acc);color:var(--on);border-color:var(--acc)}
.campos{display:grid;grid-template-columns:1fr 1fr;gap:10px;margin-top:10px}
label{display:grid;gap:4px;font-size:.85rem;color:var(--mut)}
input{font:inherit;min-height:44px;padding:0 10px;border-radius:10px;border:1px solid var(--line);background:var(--bg);color:var(--ink);width:100%}
input:invalid{border-color:var(--bad)}
.acciones{display:grid;gap:8px;margin-top:4px}
.motor{display:grid;grid-template-columns:repeat(4,1fr);gap:6px}
.motor .fix{grid-column:1/-1;margin-top:4px}
.aviso{color:var(--bad);font-weight:600}
pre{margin:8px 0 0;max-height:150px;overflow:auto;font:.78rem/1.4 ui-monospace,Menlo,Consolas,monospace;white-space:pre-wrap;color:var(--mut)}
summary{cursor:pointer;font-weight:600}
</style>
</head>
<body>
<div class="wrap">
  <h1>Teñidor automático</h1>
  <p class="demo" id="demo" hidden>Vista previa: nada se envía al equipo. Cada acción aparece en el registro con la petición exacta.</p>

  <div class="tabs" role="tablist">
    <button role="tab" aria-selected="true" data-t="r">Rutina</button>
    <button role="tab" aria-selected="false" data-t="c">Calibración</button>
  </div>

  <section id="vRutina">
    <div class="card">
      <h2>Rutinas prehechas</h2>
      <p>Cargan sus pasos en el formulario; puedes cambiarlos antes de enviar.</p>
      <div class="row" id="presets" style="margin:0"></div>
    </div>
    <p>Elige la cubeta de cada paso. Máximo 10 pasos.</p>
    <div id="pasos"></div>
    <div class="acciones">
      <button id="agregar">Agregar paso</button>
      <p id="total" style="margin:0"></p>
      <p id="bloqueo" class="aviso" hidden>Sal del modo calibración para enviar una rutina.</p>
      <button id="enviar" class="pri">Enviar rutina</button>
    </div>
  </section>

  <section id="vCal" hidden>
    <div class="card">
      <h2>Modo calibración</h2>
      <p id="estadoCal"></p>
      <div class="row" style="margin:0">
        <button id="entrar" class="pri">Entrar</button>
        <button id="salir">Salir</button>
      </div>
    </div>
    <div id="cal">
      <div class="card">
        <h2>Yugo</h2>
        <p>Sube el yugo hasta arriba y fija ese punto. El signo menos lo sube y el más lo baja.</p>
        <div class="motor" id="g0"></div>
      </div>
      <div class="card">
        <h2>Carrusel</h2>
        <p>Alinea la cubeta 1 (azul de metileno) bajo el canasto. El signo más gira el carrusel de la cubeta 1 hacia la 2.</p>
        <p id="nota1" class="aviso" hidden>Fija primero la posición de arriba del yugo para desbloquear el carrusel.</p>
        <div class="motor" id="g1"></div>
      </div>
    </div>
  </section>

  <details open class="card">
    <summary>Registro</summary>
    <pre id="log"></pre>
  </details>
</div>

<script>
const DEMO = location.hostname !== "192.168.4.1";   // vista previa, salvo que la sirva el ESP32
const MAX = 10;
const NOMBRES = ["Azul de metileno", "Agua de lavado", "Alcohol", "Agua de lavado final"];
// Cada paso es [cubeta 1..4, inmersión en s, escurrido en s].
const PRESETS = {
  "Tinción intensa": [[1,45,5],[2,10,3],[3,5,4],[4,20,5]],
  "Tinción rápida": [[1,18,4],[2,35,3],[3,5,4],[4,10,5]]
};
let pasos = [[1,30,10]], enCalib = false, yugoFijado = false;
const $ = s => document.querySelector(s);

function log(t){ const l = $("#log"); l.textContent += t + "\n"; l.scrollTop = l.scrollHeight; }

// Manda una petición POST al ESP32; en vista previa solo la muestra.
async function enviar(ruta, datos){
  const cuerpo = new URLSearchParams(datos);
  log("POST " + ruta + "  " + cuerpo.toString());
  if (DEMO) return true;
  try {
    const r = await fetch(ruta, { method: "POST", body: cuerpo });
    log(r.status + " " + await r.text());
    return r.ok;
  } catch (e) {
    log("Sin conexión con el equipo. Revisa que el teléfono esté en su red.");
    return false;
  }
}

function dibujar(){
  $("#pasos").innerHTML = pasos.map((p, i) => `
    <div class="card">
      <div class="top"><b>Paso ${i+1}</b><button class="sm" data-del="${i}" ${pasos.length == 1 ? "disabled" : ""}>Quitar</button></div>
      <div class="seg-row">${NOMBRES.map((n, k) => `<button class="seg" data-i="${i}" data-v="${k+1}" aria-pressed="${p[0] == k+1}">${k+1}<small>${n}</small></button>`).join("")}</div>
      <div class="campos">
        <label>Inmersión (s)<input type="number" min="1" max="3600" step="1" required inputmode="numeric" value="${p[1]}" data-i="${i}" data-k="1"></label>
        <label>Escurrido (s)<input type="number" min="1" max="3600" step="1" required inputmode="numeric" value="${p[2]}" data-i="${i}" data-k="2"></label>
      </div>
    </div>`).join("");
  estado();
}

// Habilita o bloquea botones según los datos y el modo.
function estado(){
  const ok = pasos.length >= 1 && pasos.length <= MAX &&
    pasos.every(p => [1,2].every(k => Number.isInteger(p[k]) && p[k] >= 1 && p[k] <= 3600));
  $("#total").textContent = `${pasos.length} de ${MAX} pasos.`;
  $("#enviar").disabled = !ok || enCalib;
  $("#agregar").disabled = pasos.length >= MAX;
  $("#bloqueo").hidden = !enCalib;
  $("#entrar").disabled = enCalib;
  $("#salir").disabled = !enCalib;
  document.querySelectorAll("[data-m]").forEach(b => b.disabled = !enCalib || (b.dataset.m == "1" && !yugoFijado));
  $("#estadoCal").textContent = enCalib ? "Calibración activa: los motores responden a los botones." : "Fuera de calibración: los botones de motor están bloqueados.";
  $("#nota1").hidden = !(enCalib && !yugoFijado);
}

// Rutina: edición de pasos
$("#pasos").addEventListener("input", e => {
  const t = e.target;
  if (t.dataset.k) { pasos[+t.dataset.i][+t.dataset.k] = Number(t.value); estado(); }
});
$("#pasos").addEventListener("click", e => {
  const b = e.target.closest("button"); if (!b) return;
  if (b.dataset.del !== undefined) { pasos.splice(+b.dataset.del, 1); dibujar(); }
  else if (b.dataset.v) { pasos[+b.dataset.i][0] = +b.dataset.v; dibujar(); }
});
$("#agregar").onclick = () => { pasos.push([1,30,10]); dibujar(); };
$("#presets").innerHTML = Object.keys(PRESETS).map(k => `<button class="sm" data-p="${k}">${k}</button>`).join("");
$("#presets").onclick = e => { const k = e.target.dataset.p; if (k) { pasos = PRESETS[k].map(p => p.slice()); dibujar(); } };
// Formato que espera el ESP32: n, v0, i0, e0, v1, i1, e1...
$("#enviar").onclick = () => {
  const d = { n: pasos.length };
  pasos.forEach((p, i) => { d["v"+i] = p[0]; d["i"+i] = p[1]; d["e"+i] = p[2]; });
  enviar("/rutina", d);
};

// Calibración
for (const m of [0, 1]) {
  $("#g" + m).innerHTML = [-100,-50,-10,-5,5,10,50,100].map(n =>
    `<button data-m="${m}" data-n="${n}">${n > 0 ? "+" : "−"}${Math.abs(n)}</button>`).join("") +
    `<button class="fix pri" data-m="${m}" data-fix>${m == 0 ? "Fijar como arriba" : "Fijar como cubeta 1"}</button>`;
}
$("#entrar").onclick = async () => { if (await enviar("/calibracion", { accion: "entrar" })) { enCalib = true; estado(); } };
$("#salir").onclick = async () => { if (await enviar("/calibracion", { accion: "salir" })) { enCalib = false; yugoFijado = false; estado(); } };
$("#cal").addEventListener("click", async e => {
  const b = e.target.closest("button[data-m]"); if (!b) return;
  const motor = +b.dataset.m;
  if (b.dataset.fix !== undefined) {
    if (await enviar("/calibracion", { accion: "fijar", motor }) && motor == 0) { yugoFijado = true; estado(); }
  } else {
    await enviar("/calibracion", { accion: "mover", motor, pasos: +b.dataset.n });
  }
});

// Pestañas
document.querySelectorAll(".tabs button").forEach(b => b.onclick = () => {
  document.querySelectorAll(".tabs button").forEach(x => x.setAttribute("aria-selected", x === b));
  $("#vRutina").hidden = b.dataset.t !== "r";
  $("#vCal").hidden = b.dataset.t !== "c";
});

$("#demo").hidden = !DEMO;
dibujar();
</script>
</body>
</html>
)rawliteral";