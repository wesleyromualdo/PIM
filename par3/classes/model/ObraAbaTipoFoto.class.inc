<?php
/**
 * Classe de mapeamento da entidade par3.obra_aba_tipo_foto.
 *
 * @version $Id$
 * @since 2017.07.12
 */

/**
 * Par3_Model_ObraAbaTipoFoto: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Eduardo Dunice Neto <eduardoneto@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_ObraAbaTipoFoto($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_ObraAbaTipoFoto($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $atfid  - default: nextval('par3.obra_aba_tipo_foto_atfid_seq'::regclass)
 * @property string $atfdsc
 * @property string $atfstatus
 */
class Par3_Model_ObraAbaTipoFoto extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_aba_tipo_foto';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'atfid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'atfid' => null,
        'atfdsc' => null,
        'atfstatus' => null,
    );

    /**Validar Input
     * @param array $campos
     * @return bool
     */
    public function validarInput(array $campos)
    {
        //campos
        $erros['erros']['atfdsc'] = array();

        //Validações
        if($campos['atfdsc'] == '' || $campos['atfdsc'] == null){array_push($erros['erros']['atfdsc'],'O campo não pode ser vazio');}
        if($this->verificarExiste($campos)){array_push($erros['erros']['atfdsc'], 'Já existe Tipo de Foto cadastrado com essa descrição');}
        //CASO HAJA ERROS, RETORNA ARRAY DE ERROS
        foreach ($erros['erros'] as $key => $value){
            if (!empty($erros['erros'][$key])){
                return $erros;
            }
        }
        return false;
    }

    function verificarExiste($campos)
    {
        $sql = "SELECT true FROM par3.obra_aba_tipo_foto WHERE removeacento(upper(atfdsc)) ilike removeacento(upper('{$campos['atfdsc']}'))";

        return $this->pegaUm($sql) == 't';
    }

    function getComboTipoFoto($obrid){
        $sql = "select
                    atf.atfid as codigo,
                    atf.atfdsc as descricao
                from 
                    par3.obra_aba_tipo_foto atf
                inner join par3.obra_configuracao_aba_x_tipo_foto axf on axf.atfid = atf.atfid
                inner join par3.obra_configuracao_aba oca ON oca.ocaid = axf.ocaid
                inner join par3.obra_tipo otp on otp.otpid = oca.otpid
                where
                    otp.otpid = ".$obrid."
                order by
                    2";
        $res =  $this->carregar($sql);
        foreach ($res as $valor) {
            $arr[$valor['codigo']] = $valor['descricao'];
        }
        return $arr;
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
