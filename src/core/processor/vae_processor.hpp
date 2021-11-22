			// toDevice.set(0.0f);
			// const auto samplesNeded = toDevice.validSize();
			// mLock.lock();
			// const auto count = mEmitters.size();

			// const auto speakerL = glm::normalize(glm::vec2(-1, 0.5));
			// const auto speakerR = glm::normalize(glm::vec2(+1, 0.5));
			// const glm::mat2x2 speakers(
			// 	speakerL.x, speakerL.y,
			// 	speakerR.x, speakerR.y
			// );
			// const auto matrix = glm::inverse(speakers);

			// for (Size index = 0; index < count; index++) {

			// 	if (mEmitters.getLastFree() == index) { continue; }
			// 	auto& i = mEmitters[index];

			// 	if (!(i.state[Emitter::ready]))   { continue; }
			// 	if (!(i.state[Emitter::playing])) { continue; }
			// 	Clip* clip = mClips.at(i.clip);
			// 	if (clip == nullptr) { continue; }
			// 	auto& buffer = clip->data;
			// 	const auto totalLength = buffer.size();

			// 	const auto startTime = i.time;

			// 	if (!i.state[Emitter::virt]) {
			// 		const auto direction = glm::normalize(glm::vec2(i.position.x, i.position.y));
			// 		auto pan = direction * matrix;
			// 		pan[1] = tklb::clamp<float>(+direction.x, 0, 1.0);
			// 		pan[0] = tklb::clamp<float>(-direction.x, 0, 1.0);
			// 		// pan[0] = 1;
			// 		// pan[1] = 1;
			// 		// pan[0] = std::min(std::max(-i.position.x, 0.f));

			// 		if (i.state[Emitter::loop]) {
			// 			for (int c = 0; c < toDevice.channels(); c++) {
			// 				const int channel = c % buffer.channels();
			// 				for (size_t s = 0; s < samplesNeded; s++) {
			// 					toDevice[c][s] += buffer[channel][(startTime + s) % totalLength] * pan[c];
			// 				}
			// 			}
			// 		} else {
			// 			const auto length = std::min(samplesNeded, totalLength - startTime);
			// 			for (int c = 0; c < toDevice.channels(); c++) {
			// 				const int channel = c % buffer.channels();
			// 				for (size_t s = 0; s < length; s++) {
			// 					toDevice[c][s] += buffer[channel][startTime + s] * pan[c];
			// 				}
			// 			}
			// 		}
			// 	}

			// 	if (!i.state[Emitter::loop] && totalLength <= startTime + samplesNeded) {
			// 		// stop if not looping and end is reached
			// 		i.time = 0;
			// 		i.state[Emitter::playing] = false;
			// 	} else {
			// 		// move time forward
			// 		i.time = startTime + samplesNeded % totalLength;
			// 	}
			// }
			// mLock.unlock();

			// mTime += samplesNeded;
