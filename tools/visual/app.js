// Helper to convert number to zero-padded 8-digit uppercase hex
function toHex(value) {
	return '0x' + value.toString(16).toUpperCase().padStart(8, '0');
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

function decodeRegisters(buffer) {
	if (buffer.byteLength < 148) {
		throw new Error('Buffer too small to contain Registers struct');
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
	const fiqFields = ['r8_fiq', 'r9_fiq', 'r10_fiq', 'r11_fiq', 'r12_fiq', 'r13_fiq', 'r14_fiq', 'SPSR_fiq'];
	fiqFields.forEach((name) => {
		result[name] = view.getUint32(offset, true);
		offset += 4;
	});

	// Read r13_svc, r14_svc, SPSR_svc (3)
	const svcFields = ['r13_svc', 'r14_svc', 'SPSR_svc'];
	svcFields.forEach((name) => {
		result[name] = view.getUint32(offset, true);
		offset += 4;
	});

	// Read r13_abt, r14_abt, SPSR_abt (3)
	const abtFields = ['r13_abt', 'r14_abt', 'SPSR_abt'];
	abtFields.forEach((name) => {
		result[name] = view.getUint32(offset, true);
		offset += 4;
	});

	// Read r13_irq, r14_irq, SPSR_irq (3)
	const irqFields = ['r13_irq', 'r14_irq', 'SPSR_irq'];
	irqFields.forEach((name) => {
		result[name] = view.getUint32(offset, true);
		offset += 4;
	});

	// Read r13_und, r14_und, SPSR_und (3)
	const undFields = ['r13_und', 'r14_und', 'SPSR_und'];
	undFields.forEach((name) => {
		result[name] = view.getUint32(offset, true);
		offset += 4;
	});

	// Read CPSR (1 uint32_t)
	result.CPSR = view.getUint32(offset, true);
	offset += 4;

	return result;
}

function sayHello() {
	alert('Hello, world!');

	fetch('data/registers.bin')
		.then((res) => res.arrayBuffer())
		.then((buffer) => {
			// Registers
			const registers = decodeRegisters(buffer);

			const tbodyRegisters = document.querySelector('#registersTable tbody');

			// Add r[0] ... r[15] rows first
			registers.r.forEach((val, i) => {
				const tr = document.createElement('tr');
				tr.innerHTML = `<td>r[${i}]</td><td>${toHex(val)}</td>`;
				tbodyRegisters.appendChild(tr);
			});

			// Add all other keys except 'r'
			Object.keys(registers).forEach((key) => {
				if (key !== 'r') {
					const tr = document.createElement('tr');
					tr.innerHTML = `<td>${key}</td><td>${toHex(registers[key])}</td>`;
					tbodyRegisters.appendChild(tr);
				}
			});
		})
		.catch(console.error);

	fetch('data/program.bin')
		.then((res) => res.arrayBuffer())
		.then((buffer) => {
			// Program
			const program = parseUint32Array(buffer);

			const tbodyProgram = document.querySelector('#programTable tbody');

			program.reverse().forEach((val, i) => {
				const tr = document.createElement('tr');
				tr.innerHTML = `<td>${toHex((program.length - i - 1) * 4)}</td><td>${toHex(val)}</td>`;
				tbodyProgram.appendChild(tr);
			});
		})
		.catch(console.error);

	fetch('data/memory.bin')
		.then((res) => res.arrayBuffer())
		.then((buffer) => {
			// Memory
			const memory = parseUint32Array(buffer);

			const tbodyMemory = document.querySelector('#memoryTable tbody');

			memory.reverse().forEach((val, i) => {
				const tr = document.createElement('tr');
				tr.innerHTML = `<td>${toHex((memory.length - i - 1) * 4)}</td><td>${toHex(val)}</td>`;
				tbodyMemory.appendChild(tr);
			});
		})
		.catch(console.error);
}
