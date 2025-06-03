function toHex(value) {
  return "0x" + value.toString(16).toUpperCase();
}

function toBinary(value) {
  return "0b" + value.toString(2).padStart(32, "0");
}

function decodeMemory(buffer) {
  const view = new DataView(buffer);
  let offset = 0;
  const result = {};

  let bios = [];
  for (let i = 0; i < 0x4000; i++) {
    bios.push(view.getUint8(offset, true));
    offset += 1;
  }
  result["BIOS"] = bios;

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
  for (let i = 0; i < 16; i++) {
    result["r" + i] = view.getUint32(offset, true);
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

function ReadRegisterData(snapshotNum) {
  let dir = "data/snapshot_" + snapshotNum + "/";
  console.log("generating" + dir);

  return fetch(dir + "registers.bin")
    .then((res) => res.arrayBuffer())
    .then((buffer) => {
      console.log("Loading registers: " + dir + "registers.bin");
      return decodeRegisters(buffer);
    })
    .catch(console.error);
}

function ReadMemoryData(snapshotNum) {
  let dir = "data/snapshot_" + snapshotNum + "/";
  console.log("generating" + dir);

  return fetch(dir + "memory.bin")
    .then((res) => res.arrayBuffer())
    .then((buffer) => {
      console.log("Loading memory: " + dir + "memory.bin");
      return decodeMemory(buffer);
    })
    .catch(console.error);
}
