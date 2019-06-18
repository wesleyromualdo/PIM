<?php

    if( $_SESSION['sislayoutbootstrap'] == 'zimec' ){
        $html_agrupador = <<<HTML
            <style>
                .select_multiple{
                    height: 130px !important;
                    width: 200px !important;
                }
            </style>

            <script type="text/javascript" src="../includes/agrupador.js"></script>

            <div class="form-group " style="margin-left:1px;">
                <div class="row">
                    <div class="col-lg-2 text-left" style="margin-left:2px;">
                        <input id="busca{NOME_ORIGEM}" name="busca{NOME_ORIGEM}" class="normal" onkeydown="return movimentoComtecla{NOME_ORIGEM}(event, '', this ,'{NOME_ORIGEM}', '{NOME_DESTINO}');" onkeyup="limitaConteudoAgrupador{NOME_ORIGEM}(this,'{NOME_ORIGEM}', event,'{NOME_DESTINO}' );" onfocus="this.value = '';"  type="text" title=""  value="Pesquisar campo..." style="width: 200px; " />
                    </div>
                </div>
                <div class="row" style="margin-top:2px;">
                    <div class="col-lg-3 text-left" style="margin-left:2px; width:213px !important;">
                        <select id="{NOME_ORIGEM}" name="{NOME_ORIGEM}[]" multiple="multiple" size="7"  onkeydown="return movimentoComtecla{NOME_ORIGEM}(event, 'envia', this , '{NOME_ORIGEM}', '{NOME_DESTINO}');" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );" class="combo campoEstilo select_multiple"></select>
                    </div>

                    <div class="col-lg-1 text-center" style="margin-top:2px; width:45px;">
                        <button type="button" class="btn btn-sm btn-mover-coluna" onClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"><i class="glyphicon glyphicon-arrow-right"></i></button>
                        <div class="clearfix"style="margin-bottom:3px;"></div>
                        <button type="button" class="btn btn-sm btn-mover-coluna" onClick="moveAllOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"><i class="glyphicon glyphicon-forward"></i></button>
                        <div class="clearfix"style="margin-bottom:3px;"></div>
                        <button type="button" class="btn btn-sm btn-mover-coluna" onClick="moveAllOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, ''); document.getElementById( '{NOME_ORIGEM}' )"><i class="glyphicon glyphicon-backward"></i></button>
                        <div class="clearfix"style="margin-bottom:3px;"></div>
                        <button type="button" class="btn btn-sm btn-mover-coluna" onClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' );  document.getElementById( '{NOME_ORIGEM}' )"><i class="glyphicon glyphicon-arrow-left"></i></button>
                    </div>
                    <div class="col-lg-3 text-left" style="width:213px !important;">
                         <select id="{NOME_DESTINO}" name="{NOME_DESTINO}[]" multiple="multiple" size="7" onkeydown="return movimentoComtecla{NOME_ORIGEM}(event, 'retorna', this ,  '{NOME_ORIGEM}', '{NOME_DESTINO}');" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); document.getElementById( '{NOME_ORIGEM}' )" class="combo campoEstilo  select_multiple"></select>
                    </div>
                    <div class="col-lg-1 text-left" style="margin-top:40px;">
                        <button type="button" class="btn btn-sm" onClick="subir( document.getElementById( '{NOME_DESTINO}' ) );"><i class="glyphicon glyphicon-arrow-up"></i></button>
                        <div class="clearfix"style="margin-bottom:5px;"></div>
                        <button type="button" class="btn btn-sm" onClick="descer( document.getElementById( '{NOME_DESTINO}' ) );"><i class="glyphicon glyphicon-arrow-down"></i></button>
                    </div>
                </div>
            </div>
HTML;

    }else{

        $html_agrupador = <<<HTML
            <script type="text/javascript" src="../includes/agrupador.js"></script>

            <table>
                <tr>
                    <td colspan=4>
                        <input id="busca{NOME_ORIGEM}" name="busca{NOME_ORIGEM}" class="normal" onkeydown="return movimentoComtecla{NOME_ORIGEM}(event, '', this, '{NOME_ORIGEM}', '{NOME_DESTINO}');" onkeyup="limitaConteudoAgrupador{NOME_ORIGEM}(this, '{NOME_ORIGEM}', event, '{NOME_DESTINO}');" onfocus="this.value = '';"  type="text" title=""  value="Pesquisar campo..." style="width: 200px;" />
                    </td>
                </tr>
                <tr valign="middle">
                    <td>
                        <select id="{NOME_ORIGEM}" name="{NOME_ORIGEM}[]" multiple="multiple" size="7" onkeydown="return movimentoComtecla{NOME_ORIGEM}(event, 'envia', this, '{NOME_ORIGEM}', '{NOME_DESTINO}');" onDblClick="moveSelectedOptions(document.getElementById('{NOME_ORIGEM}'), document.getElementById('{NOME_DESTINO}'), true, '');" class="combo campoEstilo"></select>
                    </td>
                    <td>
                        <img src="../imagens/rarrow_one.gif" style="padding: 2px" onClick="moveSelectedOptions(document.getElementById('{NOME_ORIGEM}'), document.getElementById('{NOME_DESTINO}'), true, '');"/><br/>
                        <img src="../imagens/rarrow_all.gif" style="padding: 2px" onClick="moveAllOptions(document.getElementById('{NOME_ORIGEM}'), document.getElementById('{NOME_DESTINO}'), true, '');"/><br/>
                        <img src="../imagens/larrow_all.gif" style="padding: 2px" onClick="moveAllOptions(document.getElementById('{NOME_DESTINO}'), document.getElementById('{NOME_ORIGEM}'), true, ''); document.getElementById('{NOME_ORIGEM}');"/><br/>
                        <img src="../imagens/larrow_one.gif" style="padding: 2px" onClick="moveSelectedOptions(document.getElementById('{NOME_DESTINO}'), document.getElementById('{NOME_ORIGEM}'), true, ''); document.getElementById('{NOME_ORIGEM}')"/><br/>
                    </td>
                    <td>
                        <select id="{NOME_DESTINO}" name="{NOME_DESTINO}[]" multiple="multiple" size="7" onkeydown="return movimentoComtecla{NOME_ORIGEM}(event, 'retorna', this, '{NOME_ORIGEM}', '{NOME_DESTINO}');" onDblClick="moveSelectedOptions(document.getElementById('{NOME_DESTINO}'), document.getElementById('{NOME_ORIGEM}'), true, ''); document.getElementById('{NOME_ORIGEM}')" class="combo campoEstilo"></select>
                    </td>
                    <td>
                        <img src="../imagens/uarrow.gif" style="padding: 5px" onClick="subir(document.getElementById('{NOME_DESTINO}'));"/><br/>
                        <img src="../imagens/darrow.gif" style="padding: 5px" onClick="descer(document.getElementById('{NOME_DESTINO}'));"/><br/>
                    </td>
                </tr>
            </table>
HTML;
    }

    $html_agrupador .= <<<EOF
        <script type="text/javascript" language="javascript">
            limitarQuantidade( document.getElementById( '{NOME_DESTINO}' ), {QUANTIDADE_DESTINO} );
            limitarQuantidade( document.getElementById( '{NOME_ORIGEM}' ), {QUANTIDADE_ORIGEM} );
            {POVOAR_ORIGEM}
            {POVOAR_DESTINO}

           //sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );
            //TECLAS : 13 - enter  |  38 - para cima   |  40 - para baixo

            function movimentoComtecla{NOME_ORIGEM}(tecla, tipo, local, campoOrigem, campoDestino){
                if(window.event){ // Se IE
                    codigoTecla = tecla.keyCode;
                }else if(tecla.which){
                    codigoTecla = tecla.which;
                }

                var campo       = local;
                var campoBusca  = document.getElementById( 'busca'+campoOrigem );
                var charTecla   = String.fromCharCode(codigoTecla);
                var agrupador   = document.getElementById(campoOrigem);
                var destino     = document.getElementById(campoDestino);
                var selecao     = document.getElementById(campoOrigem).selectedIndex;

                if(tecla.keyCode == 40){ // para baixo
                    if(agrupador.selectedIndex == -1){
                        agrupador[0].selected = true;
                    }else{
                        for (i=0;i < agrupador.length; i++){
                            if(agrupador[i].selected == true) {
                                if(agrupador[i+1] != null){
                                    agrupador[i].selected = false;
                                    agrupador[i+1].selected = true;
                                    break;
                                }else{
                                    agrupador[i].selected = false;
                                    break;
                                }
                            }
                        }
                    }

                }else if (tecla.keyCode == 13){ // tecla enter

                    moveSelectedOptions( agrupador, destino , true, '' );

                }else if(tecla.keyCode == 38){ // para cima

                    if(agrupador.selectedIndex == -1){
                        for (i=0;i < agrupador.length;i++){
                            agrupador[i].selected = true;
                            break;
                        }
                    }else{

                        for (i=agrupador.length;i > 0;i--){
                            if(agrupador[i] != null ){
                                if (agrupador[i].selected == true) {
                                    if (agrupador[i - 1] != null){
                                        agrupador[i].selected = false;
                                        agrupador[i - 1].selected = true;
                                        break;
                                    }else{
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
                return true;
            }

            var objs{NOME_ORIGEM} = new Object;

            objs{NOME_ORIGEM}.dados = {
                valor : new Array(),
                texto : new Array()
            };

            var agrupador{NOME_ORIGEM} = document.getElementById('{NOME_ORIGEM}');

            for (cont = 0; cont < agrupador{NOME_ORIGEM}.length; cont++){
                objs{NOME_ORIGEM}.dados.valor[cont] = agrupador{NOME_ORIGEM}[cont].value;
                objs{NOME_ORIGEM}.dados.texto[cont] = agrupador{NOME_ORIGEM}[cont].text;
            }

            function limitaConteudoAgrupador{NOME_ORIGEM}(busca, nomeAgrupador, tecla, nomeAgrupador2){
                var selectAgrupador = document.getElementById(nomeAgrupador);
                var selectAgrupadorDestino = document.getElementById(nomeAgrupador2);
                var busca = busca.value;

                busca = busca.toLowerCase();
                if (tecla.keyCode != 40 && tecla.keyCode != 38 && tecla.keyCode != 13){
                    if (busca != ''){ // SE BUSCAR ALGO
                        for (i = selectAgrupador.length - 1; i >= 0; i--) {
                            selectAgrupador.remove(i);
                        }

                        for (var cont = 0; cont < objs{NOME_ORIGEM}.dados.texto.length; cont++){
                            var banco = objs{NOME_ORIGEM}.dados.texto[cont].toLowerCase();
                            if (banco.indexOf(busca) != '-1'){
                                var opcoes = document.createElement('option');
                                opcoes.text = objs{NOME_ORIGEM}.dados.texto[cont];
                                opcoes.value = objs{NOME_ORIGEM}.dados.valor[cont];

                                try{
                                    selectAgrupador.add(opcoes, null);
                                } catch (ex){
                                    selectAgrupador.add(opcoes); // IE only
                                }

                                for (i = selectAgrupadorDestino.length - 1; i >= 0; i--){
                                    if (selectAgrupadorDestino[i].text == objs{NOME_ORIGEM}.dados.texto[cont]){

                                        for (x = selectAgrupador.length - 1; x >= 0; x--) {
                                            if (selectAgrupador[x].text == selectAgrupadorDestino[i].text){

                                                selectAgrupador.remove(x);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }else{
                        for (i = selectAgrupador.length - 1; i >= 0; i--) {
                            selectAgrupador.remove(i);
                        }
                        for (cont = 0; cont < objs{NOME_ORIGEM}.dados.texto.length; cont++){
                            var opcoes = document.createElement('option');
                            opcoes.text = objs{NOME_ORIGEM}.dados.texto[cont];
                            opcoes.value = objs{NOME_ORIGEM}.dados.valor[cont];

                            try{
                                selectAgrupador.add(opcoes, null);
                            }catch(ex){
                                selectAgrupador.add(opcoes); // IE only
                            }
                        }

                        for (i = selectAgrupadorDestino.length - 1; i >= 0; i--){
                            for (x = selectAgrupador.length - 1; x >= 0; x--){
                                if (selectAgrupadorDestino[i].text == selectAgrupador[x].text){
                                    selectAgrupador.remove(x);
                                }
                            }
                        }
                    }
                }
            }
        </script>
EOF;

class Agrupador {

    private $destino = array();
    private $formulario = '';
    private $origem = array();
    private $html = '';

    public function __construct($formulario, $html = null) {
        $this->formulario = (string) $formulario;
        global $html_agrupador;
        $this->html = $html ? $html : $html_agrupador;
    }

    /**
     * Exibe o código html do agrupador. Essa função deve ser chamada depois
     * de configurar os campos de origem e destino.
     */
    public function exibir() {
        $html = $this->html;
        $html = str_replace('{NOME_FORMULARIO}', $this->formulario, $html);

        // configura o html para o campo de origem
        if (is_array($this->origem['dados'])) {
            $javascript = '';
            foreach ($this->origem['dados'] as $registro) {
                if (is_array($this->destino['dados'])) {
                    foreach ($this->destino['dados'] as $registroDestino) {
                        if ($registro['codigo'] == $registroDestino['codigo']) {
                            continue 2;
                        }
                    }
                }
                $javascript .= sprintf(
                    "inserirItem( '%s', '%s', '%s', '%s' );", $this->formulario, $this->origem['nome'], $registro['codigo'], $registro['descricao']
                );
            }
        }
        $html = str_replace('{POVOAR_ORIGEM}', $javascript, $html);
        $html = str_replace('{NOME_ORIGEM}', $this->origem['nome'], $html);
        $html = str_replace('{QUANTIDADE_ORIGEM}', $this->origem['quantidade'], $html);

        // configura o html para o campo de destino
        $javascript = '';
        if (is_array($this->destino['dados'])) {
            $javascript = '';
            foreach ($this->destino['dados'] as $registro) {
                $javascript .= sprintf(
                    "inserirItem( '%s', '%s', '%s', '%s' );", $this->formulario, $this->destino['nome'], $registro['codigo'], $registro['descricao']
                );
            }
        }
        $html = str_replace('{POVOAR_DESTINO}', $javascript, $html);
        $html = str_replace('{NOME_DESTINO}', $this->destino['nome'], $html);
        $html = str_replace('{QUANTIDADE_DESTINO}', $this->destino['quantidade'], $html);
         print $html;
    }

    /**
     * Modifica a configuração do campo que irá agrupar os itens
     * selecionados no campo de origem.
     *
     * @param string $nome Nome do campo no formulário.
     * @param integer $quantidade Quantidade máxima de itens permitido para o campo.
     * @param mixed $dados Comando sql que carrega os itens previamente selecionados para o campo ou uma matriz.
     * @return void
     */
    public function setDestino($nome, $quantidade = null, $dados = null) {
        global $db;
        if ($dados == null) {
            global ${$nome};
            $dados = ${$nome};
        }
        $this->destino = array(
            'nome' => $nome,
            'quantidade' => $quantidade ? $quantidade : 'null',
        );
        if (!empty($dados)) {
            if (is_array($dados)) {
                $this->destino['dados'] = $dados;
            } else if (is_string($dados)) {
                $this->destino['dados'] = $db->carregar($dados);
            }
        }
    }

    /**
     * Altera a configuração do campo que contém todos os itens possíveis.
     *
     * @param string $nome Nome do campo no formulário.
     * @param integer $quantidade Quantidade máxima de itens permitido para o campo.
     * @param mixed $dados Comando sql que carrega os itens previamente selecionados para o campo ou uma matriz.
     * @return void
     */
    public function setOrigem($nome, $quantidade = null, $dados = null) {
        global $db;
        $this->origem = array(
            'nome' => $nome,
            'quantidade' => $quantidade ? $quantidade : 'null',
        );
        if (!empty($dados)) {
            if (is_array($dados)) {
                $this->origem['dados'] = $dados;
            } else if (is_string($dados)) {
                $this->origem['dados'] = $db->carregar($dados);
            }
        }
    }
}

?>
