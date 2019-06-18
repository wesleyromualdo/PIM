<?php 

/**
 * Para as obras de convenio, todas devem possuir os mesmos dados na aba validação
 * Essa função garante que a alteração feita em uma obra de converio, todas as outras que compatilham o mesmo convenio
 * terão a validação igual.
 * @param $obrid
 * @param $arDado
 * @param $arCamposNulo
 */

function espelhaObrasConvenio($obrid, $arDado, $arCamposNulo)
{
    $validacao = new Validacao();
    $obra = new Obras();
    $obra->carregarPorIdCache($obrid);
    if ($obra->tooid == 2) {
        $obras = $obra->pegaObrasConvenio($obra->numconvenio, $obra->obranoconvenio);
        foreach ($obras as $obra) {
            $arDadoV = $arDado;
            $validacaoObra = $validacao->pegaValidacaoObra($obra['obrid']);
            if ($validacaoObra)
                $validacao = new Validacao($validacaoObra['vldid']);
            else
                $validacao = new Validacao();
            $arDadoV['obrid'] = $obra['obrid'];
            $validacao->popularDadosObjeto($arDadoV)->salvar(true, true, $arCamposNulo);
        }
        $validacao->commit();
    }
}

