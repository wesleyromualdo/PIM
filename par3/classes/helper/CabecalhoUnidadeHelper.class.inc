<?php
/**
 * Created by PhpStorm.
 * User: leooliveira
 * Date: 9/12/2017
 * Time: 3:07 PM
 */


/**
 * Class Par3_Helper_CabecalhoUnidadeHelper
 * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
 * @description <Este helper contem funções que podem ajudar na manipulação do cabeçalhos das unidade "Menu inicial de navegação">
 */
class Par3_Helper_CabecalhoUnidadeHelper extends Modelo {

     /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param $mostrar_proximo boolean (se for true ele mostra o próximo menu)
     * @description:
     * # - Esta função mostra o próximo menu, quando existe a possibilidade de avançar.
     * # - ela deve ser inserida em um bloco java script com um read do jquery
     * # - ela deve ser carregado quando existe um menu após o que está ativo e quando pode ser visto o próximo menu do processo.
     * #Exemplo:
     * #<script type="text/javascript">
     * #$( document ).ready(function() {
     * #<?php echo menuInicialNavegacaoMostrarProximo($mostrar_proximo);?>
     * #});
     * #</script>
     * #
     */
     public function menuInicialNavegacaoMostrarProximo($mostrar_proximo){
        if($mostrar_proximo ==  true || $mostrar_proximo == 1){
            echo "setTimeout(js_mostraProximoMenuInicialNavegacao,1000);
                 function js_mostraProximoMenuInicialNavegacao(){
                 var proximo = $('#btn-proximo-nav');
                 proximo.trigger('click');
                }";
        }
      }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @description:
     * # - Esta função mostra o menu anterioir.
     * # - ela deve ser carregado quando existe um menu anterior que deve ser mostrado.
     * #Exemplo:
     * #<script type="text/javascript">
     * #$( document ).ready(function() {
     * #<?php echo menuInicialNavegacaoMostrarProximo($mostrar_proximo);?>
     * #});
     * #</script>
     * #
     */
     public function menuInicialNavegacaoMostrarAnterior(){
        echo "setTimeout(js_mostraAnteriorMenuInicialNavegacao,2000);
                 function js_mostraAnteriorMenuInicialNavegacao(){
                 var anterior = $('#btn-anterior-nav');
                 anterior.trigger('click');
                }";

     }



}