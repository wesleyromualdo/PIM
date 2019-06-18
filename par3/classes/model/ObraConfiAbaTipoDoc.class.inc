<?php
/**
 * Classe de mapeamento da entidade par3.obra_confi_aba_x_tipo_doc.
 *
 * @version $Id$
 * @since 2017.07.13
 */

/**
 * Par3_Model_ObraConfiAbaTipoDoc: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Eduardo Dunice Neto <eduardoneto@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Obra_confi_aba_x_tipo_doc($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Obra_confi_aba_x_tipo_doc($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property string $octid
 * @property int $otdid
 * @property int $ocaid
 * @property string $octstatus
 * @property string $octordem
 */
class Par3_Model_ObraConfiAbaTipoDoc extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_confi_aba_x_tipo_doc';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'octid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'otdid' => array('tabela' => 'par3.obra_tipo_documento', 'pk' => 'otdid'),
        'ocaid' => array('tabela' => 'par3.obra_configuracao_aba', 'pk' => 'ocaid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'octid' => null,
        'otdid' => null,
        'ocaid' => null,
        'octstatus' => null,
        'octordem' => null,
    );

    /**Validar Input
     * @param array $campos
     * @return bool
     */
    public function validarInput(array $campos)
    {
        //campos
        $erros['erros']['otdid']                        = array();
        $erros['erros']['ocaid']                        = array();
        $erros['erros']['octordem'][$campos['otdid']]   = array();

        //Validações
        if($campos['otdid'] == '' || $campos['otdid'] == null){array_push($erros['erros']['otdid'],'O campo não pode ser vazio');}
        if($campos['ocaid'] == '' || $campos['ocaid'] == null){array_push($erros['erros']['ocaid'],'O campo não pode ser vazio');}
        if($campos['octordem'] == '' || $campos['octordem'] == null){array_push($erros['erros']['octordem'][$campos['otdid']],'O campo não pode ser vazio');}
        //CASO HAJA ERROS, RETORNA ARRAY DE ERROS
        foreach ($erros['erros'] as $key => $value){
            if($key == 'octordem'){
                if (!empty($erros['erros'][$key][$campos['otdid']])){
                    return $erros;
                }
            }else{
                if (!empty($erros['erros'][$key])){
                    return $erros;
                }
            }
        }
        return false;
    }

    public function inativarTodos($ocaid)
    {
        $sql = "UPDATE par3.obra_confi_aba_x_tipo_doc SET octstatus = 'I' WHERE ocaid = $ocaid";

        $this->executar($sql);
        $this->commit();
    }

    /**
     * Método de transformação de valores e validações adicionais de dados.
     *
     * Este método tem as seguintes finalidades:
     * a) Transformação de dados, ou seja, alterar formatos, remover máscaras e etc
     * b) A segunda, é a validação adicional de dados. Se a validação falhar, retorne false, se não falhar retorne true.
     *
     * @return bool
     */
    public function antesSalvar()
    {
        // -- Implemente suas transformações de dados aqui

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

}
