function readHex(hex) {
  return parseInt(hex);
}

// Helper to convert number to zero-padded 8-digit uppercase hex
function toHexPadded(value) {
  return "0x" + value.toString(16).toUpperCase().padStart(8, "0");
}

function toHex(value) {
  return "0x" + value.toString(16).toUpperCase();
}

function parseUint32Array(buffer) {
  const dataView = new DataView(buffer);
  const length = buffer.byteLength / 4;
  const result = new Uint32Array(length);

  for (let i = 0; i < length; i++) {
    result[i] = dataView.getUint32(i * 4, true); // true = little-endian
  }

  return result;
}

function parseCharArray(buffer) {
  const dataView = new DataView(buffer);
  const length = buffer.byteLength;
  const result = new Uint8Array(length);

  for (let i = 0; i < length; i++) {
    result[i] = dataView.getUint8(i, true); // true = little-endian
  }

  return result;
}

function decodeRegisters(buffer) {
  if (buffer.byteLength < 148) {
    throw new Error("Buffer too small to contain Registers struct");
  }

  const view = new DataView(buffer);
  let offset = 0;
  const result = {};

  // Read r[16]
  result.r = [];
  for (let i = 0; i < 16; i++) {
    result.r.push(view.getUint32(offset, true));
    offset += 4;
  }

  // Read r8_fiq to SPSR_fiq (8 uint32_t)
  const fiqFields = [
    "r8_fiq",
    "r9_fiq",
    "r10_fiq",
    "r11_fiq",
    "r12_fiq",
    "r13_fiq",
    "r14_fiq",
    "SPSR_fiq",
  ];
  fiqFields.forEach((name) => {
    result[name] = view.getUint32(offset, true);
    offset += 4;
  });

  // Read r13_svc, r14_svc, SPSR_svc (3)
  const svcFields = ["r13_svc", "r14_svc", "SPSR_svc"];
  svcFields.forEach((name) => {
    result[name] = view.getUint32(offset, true);
    offset += 4;
  });

  // Read r13_abt, r14_abt, SPSR_abt (3)
  const abtFields = ["r13_abt", "r14_abt", "SPSR_abt"];
  abtFields.forEach((name) => {
    result[name] = view.getUint32(offset, true);
    offset += 4;
  });

  // Read r13_irq, r14_irq, SPSR_irq (3)
  const irqFields = ["r13_irq", "r14_irq", "SPSR_irq"];
  irqFields.forEach((name) => {
    result[name] = view.getUint32(offset, true);
    offset += 4;
  });

  // Read r13_und, r14_und, SPSR_und (3)
  const undFields = ["r13_und", "r14_und", "SPSR_und"];
  undFields.forEach((name) => {
    result[name] = view.getUint32(offset, true);
    offset += 4;
  });

  // Read CPSR (1 uint32_t)
  result.CPSR = view.getUint32(offset, true);
  offset += 4;

  return result;
}

function Generate(snapshotNum) {
  let dir = "data/snapshot_" + snapshotNum + "/";
  console.log("generating" + dir);

  fetch(dir + "registers.bin")
    .then((res) => res.arrayBuffer())
    .then((buffer) => {
      console.log("Loading registers: " + dir + "registers.bin");

      // Registers
      const tbodyRegisters = document.querySelector("#registersTable tbody");
      tbodyRegisters.innerHTML = "";

      const registers = decodeRegisters(buffer);

      // Add r[0] ... r[15] rows first
      registers.r.forEach((val, i) => {
        const tr = document.createElement("tr");
        tr.innerHTML = `<td>r[${i}]</td><td>${toHex(val)}</td>`;
        tbodyRegisters.appendChild(tr);
      });

      // Add all other keys except 'r'
      Object.keys(registers).forEach((key) => {
        if (key !== "r") {
          const tr = document.createElement("tr");
          tr.innerHTML = `<td>${key}</td><td>${toHex(registers[key])}</td>`;
          tbodyRegisters.appendChild(tr);
        }
      });
    })
    .catch(console.error);

  fetch(dir + "program.bin")
    .then((res) => res.arrayBuffer())
    .then((buffer) => {
      console.log("Loading program");

      // Program
      const program = parseUint32Array(buffer);

      const tbodyProgram = document.querySelector("#programTable tbody");
      tbodyProgram.innerHTML = "";

      const program_start = document.getElementById(
        "program_start_address"
      ).value;
      const program_end = document.getElementById("program_end_address").value;

      const program_start_i = readHex(program_start) / 4;
      const program_end_i = Math.min(readHex(program_end) / 4, program.length);

      const table_values = [];
      for (let i = program_start_i; i < program_end_i; i++) {
        table_values.push([i, program[i]]);
      }

      table_values.reverse().forEach((val) => {
        const tr = document.createElement("tr");
        tr.innerHTML = `<td>${toHexPadded(val[0] * 4)}</td><td>${toHexPadded(
          val[1]
        )}</td>`;
        tbodyProgram.appendChild(tr);
      });
    })
    .catch(console.error);

  fetch(dir + "memory.bin")
    .then((res) => res.arrayBuffer())
    .then((buffer) => {
      console.log("Loading memory");

      // Memory
      const memory = parseCharArray(buffer);

      const tbodyMemory = document.querySelector("#memoryTable tbody");
      tbodyMemory.innerHTML = "";

      const mem_start = document.getElementById("memory_start_address").value;
      const mem_end = document.getElementById("memory_end_address").value;

      const mem_start_i = readHex(mem_start);
      const mem_end_i = Math.min(readHex(mem_end), memory.length);

      const table_values = [];
      for (let i = mem_start_i; i < mem_end_i; i++) {
        table_values.push([i, memory[i]]);
      }

      table_values.reverse().forEach((val) => {
        const tr = document.createElement("tr");
        tr.innerHTML = `<td>${toHex(val[0])}</td><td>${toHex(val[1])}</td>`;
        tbodyMemory.appendChild(tr);
      });
    })
    .catch(console.error);
}

function GenerateNextSnapshot() {
  let snapshotNum = document.getElementById("snapshotNum");
  nextSnapshotNum = parseInt(snapshotNum.value) + 1;

  Generate(nextSnapshotNum);
  console.log(nextSnapshotNum);

  snapshotNum.value = nextSnapshotNum;
}

function GeneratePrevSnapshot() {
  let snapshotNum = document.getElementById("snapshotNum");
  prevSnapshotNum = parseInt(snapshotNum.value) - 1;

  Generate(prevSnapshotNum);

  snapshotNum.value = prevSnapshotNum;
}

function GenerateCurrSnapshot() {
  let snapshotNum = document.getElementById("snapshotNum");
  SnapshotNum = parseInt(snapshotNum.value);

  Generate(SnapshotNum);

  snapshotNum.value = SnapshotNum;
}
