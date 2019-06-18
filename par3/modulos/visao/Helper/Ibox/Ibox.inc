<?php $idIbox = $this->configVisaoHelper['caixaBox']['id'];?>
<div
    class="<?php echo $this->configVisaoHelper['caixaExterna']['class']; ?>"
    id="ibox-<?= $this->configVisaoHelper['caixaBox']['id']; ?>"
>
    <input
        type="hidden"
        name="ibox[]"
        value="<?= $this->configVisaoHelper['caixaBox']['id']; ?>"
    />
    <input
        type="hidden"
        name="<?php echo $this->configVisaoHelper['caixaBox']['id']; ?>[visivel]"
        value="true"
    />
    <div class="<?php echo $this->configVisaoHelper['caixaBox']['class']; ?>">
        <div id="load-div-<?= $this->configVisaoHelper['caixaBox']['id']; ?>" class="ibox-load-div" style="background-color: black;opacity: .6;position: absolute;z-index: 9999999999999999;display:none;">
            <div class="ibox-loading-dialog-content-par3">
                <img src="../library/simec/img/loading.gif">
                <span>O sistema esta processando as informações. <br/>Por favor aguarde um momento...</span>
            </div>
        </div>
        <?php
    	// if 1 - inicio
    	if ($this->configVisaoHelper['caixaTitulo']['retirar'] == false):
    	?>
        <div class="<?php echo $this->configVisaoHelper['caixaTitulo']['class']; ?>">
            <h5>
            <?php 
            if (is_object($this->dados['conteudoTitulo']) == false) {
                echo $this->dados['conteudoTitulo'];
            } else {
                $this->dados['conteudoTitulo']->renderizarHelper();
            } 
            ?>
                <i class="fa fa-arrows-alt"></i>
            </h5>
            <div class="<?php echo $this->configVisaoHelper['caixaTituloOpcao']['class']; ?>">
                <?php if(count($this->configVisaoHelper['caixaOpcoes']) > 0):?>
                    <?php foreach($this->configVisaoHelper['caixaOpcoes'] as $opcao):?>
                        <?= $opcao->renderizarHelper()?>
                    <?php endforeach;?>
                <?php endif;?>
                <a class="collapse-link">
                    <i class="fa fa-chevron-up"></i>
                </a>

                <?php if($this->configVisaoHelper['caixaTitulo']['permitir-fechar']):?>
                    <a class="close-link remover-ibox" id="<?= $this->configVisaoHelper['caixaBox']['id']; ?>">
                        <i class="fa fa-times"></i>
                    </a>
                <?php endif;?>
            </div>
        </div>
        <?php 
        // if 1 - fim
        endif;
        ?>
        <div class="<?php echo $this->configVisaoHelper['caixaCorpo']['class']; ?>" max-height="<?php echo $this->configVisaoHelper['caixaCorpo']['max-height']?>" style="height:<?php echo $this->configVisaoHelper['caixaCorpo']['height']?>">
        <?php 
        if (is_object($this->dados['conteudoCorpo']) == false) {
            echo $this->dados['conteudoCorpo'];
        } else {
            $this->dados['conteudoCorpo']->renderizarHelper();
        } 
        ?>
        </div>
        <?php 
    	// if 2 - inicio
    	if ($this->configVisaoHelper['caixaRodape']['retirar'] == false):
    	?>
        <div class="<?php echo $this->configVisaoHelper['caixaRodape']['class']; ?>">
        <?php 
        if (is_object($this->dados['conteudoRodape']) == false) {
            echo $this->dados['conteudoRodape'];
        } else {
            $this->dados['conteudoRodape']->renderizarHelper();
        } 
        ?>
        </div>
        <?php 
    	// if 2 - fim
    	endif;
    	?>
    </div>	
</div>