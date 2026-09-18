# THIS IS WHERE YOU UPDATE ESP-IDF!!!
# Ensure version v5.5.5 to work with Arduino-ESP32 core (for now....)
# Follow releases of arduino-esp32 core here: https://github.com/espressif/arduino-esp32/releases
# sha256 hash of versions can be found here: https://hub.docker.com/r/espressif/idf/tags

FROM espressif/idf:v5.5.5@sha256:642e9fb2f5acfb900ef67ddea82b825755d8ab7e939fc1c9a988c35c70c991a7

ENV LC_ALL=C.UTF-8
ENV LANG=C.UTF-8

RUN apt-get update -y && apt-get install -y \
    usbutils \
    udev \
    minicom \
    picocom \
    zsh \
    git \
    curl \
    clangd \
    clang-format \
    clang-tidy

# Install Starship prompt and zsh plugins (I promise this is important for productivity)
RUN curl -sS https://starship.rs/install.sh | sh -s -- -y
RUN mkdir -p ~/.zsh
RUN git clone https://github.com/zsh-users/zsh-autosuggestions ~/.zsh/zsh-autosuggestions
RUN git clone https://github.com/zsh-users/zsh-syntax-highlighting.git ~/.zsh/zsh-syntax-highlighting
RUN echo 'source ~/.zsh/zsh-autosuggestions/zsh-autosuggestions.zsh' >> ~/.zshrc && \
    echo 'source ~/.zsh/zsh-syntax-highlighting/zsh-syntax-highlighting.zsh' >> ~/.zshrc && \
    echo 'eval "$(starship init zsh)"' >> ~/.zshrc

# Add ESP-IDF setup to zshrc
RUN echo "source /opt/esp/idf/export.sh > /dev/null 2>&1" >> ~/.zshrc
RUN echo "source /opt/esp/idf/export.sh > /dev/null 2>&1" >> ~/.bashrc

# OpenOCD udev rules
RUN curl https://raw.githubusercontent.com/espressif/openocd-esp32/refs/heads/master/contrib/60-openocd.rules > /etc/udev/rules.d/60-openocd.rules

# Set zsh as default shell
RUN chsh -s $(which zsh)

ENTRYPOINT [ "/opt/esp/entrypoint.sh" ]

CMD ["/bin/bash", "-c"]
